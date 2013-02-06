-- função para fechamento de caixa
CREATE OR REPLACE FUNCTION closebox(date, time, date, time) RETURNS float AS $$
DECLARE
	date_last_open ALIAS for $1; -- pega o primeiro parametro
	hour_last_open ALIAS for $2; -- pega o segundo parametro
	date_final ALIAS for $3;
	hour_final ALIAS for $4;
	value_day_open float; -- valor do caixa somente do dia
	value_return float;
	current_value float;
	current_hour varchar(8);
	total_lines integer;
BEGIN
	value_return := 0;

	total_lines := 0;

	current_value := 0;
	
	if (date_last_open = date_final) then -- se a data de inicio for igual a data final
		-- parte das movimentações
		select count(valuemovement) into total_lines from movementcash where datemovement = date_final and hourmovement >= hour_last_open and hourmovement <= hour_final and (typemovement = 'Abertura' or typemovement = 'Entrada');

		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement = date_final and hourmovement >= hour_last_open and hourmovement <= hour_final and (typemovement = 'Abertura' or typemovement = 'Entrada');
			value_return := value_return + current_value;

			total_lines := 0;
		end if;

		select count(valuemovement) into total_lines from movementcash where datemovement = date_final and hourmovement >= hour_last_open and hourmovement <= hour_final and typemovement = 'Saída';

		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement = date_final and hourmovement >= hour_last_open and hourmovement <= hour_final and typemovement = 'Saída';
			value_return := value_return - current_value;

			total_lines := 0;
		end if;

		-- parte dos pedidos

		select count(o.valueorder + o.valueshipping) into total_lines from ordered o, datecloseorder dco where dco.dateclose = date_final and dco.hourclose >= hour_last_open and dco.hourclose <= hour_final and dco.idorder = o.idorder;
		
		if (total_lines > 0) then
			select sum(o.valueorder + o.valueshipping) into current_value from ordered o, datecloseorder dco where dco.dateclose = date_final and dco.hourclose >= hour_last_open and dco.hourclose <= hour_final and dco.idorder = o.idorder;
			value_return := value_return + current_value;

			total_lines := 0;
		end if;
	else
		-- parte das movimentações
		select count(valuemovement) into total_lines from movementcash where datemovement = date_last_open and hourmovement >= hour_last_open and (typemovement = 'Abertura' or typemovement = 'Entrada');
		
		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement = date_last_open and hourmovement >= hour_last_open and (typemovement = 'Abertura' or typemovement = 'Entrada');
			value_return := value_return + current_value;

			total_lines := 0;

		end if;

		select count(valuemovement) into total_lines from movementcash where datemovement = date_last_open and hourmovement >= hour_last_open and typemovement = 'Saída';

		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement = date_last_open and hourmovement >= hour_last_open and typemovement = 'Saída'; -- aqui não pega fechamentos pois já sei que o fechamento foi após o dia
			value_return := value_return - current_value;

			total_lines := 0;
		end if;

		select count(valuemovement) into total_lines from movementcash where datemovement = date_final and hourmovement <= hour_final and typemovement = 'Saída';

		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement = date_final and hourmovement <= hour_final and typemovement = 'Saída';
			value_return := value_return - current_value;

			total_lines := 0;
		end if;

		select count(valuemovement) into total_lines from movementcash where datemovement = date_final and hourmovement <= hour_final and typemovement = 'Entrada'; -- aqui não pega aberturas pois já sei que a abertura foi após o dia

		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement = date_final and hourmovement <= hour_final and typemovement = 'Entrada'; -- aqui não pega aberturas pois já sei que a abertura foi após o dia
			value_return := value_return + current_value;

			total_lines := 0;
		end if;

		select count(valuemovement) into total_lines from movementcash where datemovement > date_last_open and datemovement < date_final and typemovement = 'Entrada';

		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement > date_last_open and datemovement < date_final and typemovement = 'Entrada';
			value_return = value_return + current_value;

			total_lines := 0;
		end if;

		select count(valuemovement) into total_lines from movementcash where datemovement > date_last_open and datemovement < date_final and typemovement = 'Saída';

		if (total_lines > 0) then
			select sum(valuemovement) into current_value from movementcash where datemovement > date_last_open and datemovement < date_final and typemovement = 'Saída';
			value_return = value_return - current_value;

			total_lines := 0;
		end if;


		-- parte dos pedidos
		select count(o.valueorder + o.valueshipping) into total_lines from ordered o, datecloseorder dco where dco.dateclose = date_last_open and dco.hourclose >= hour_last_open and dco.idorder = o.idorder;

		if (total_lines > 0) then
			select sum(o.valueorder + o.valueshipping) into current_value from ordered o, datecloseorder dco where dco.dateclose = date_last_open and dco.hourclose >= hour_last_open and dco.idorder = o.idorder;
			value_return := value_return + current_value;

			total_lines := 0;
		end if;

		select count(o.valueorder + o.valueshipping) into total_lines from ordered o, datecloseorder dco where dco.dateclose = date_final and dco.hourclose <= hour_final and dco.idorder = o.idorder;

		if (total_lines > 0) then
			select sum(o.valueorder + o.valueshipping) into current_value from ordered o, datecloseorder dco where dco.dateclose = date_final and dco.hourclose <= hour_final and dco.idorder = o.idorder;
			value_return := value_return + current_value;

			total_lines := 0;
		end if;

		select count(o.valueorder + o.valueshipping) into total_lines from ordered o, datecloseorder dco where dco.dateclose > date_last_open and dco.dateclose < date_final and dco.idorder = o.idorder;

		if (total_lines > 0) then
			select sum(o.valueorder + o.valueshipping) into current_value from ordered o, datecloseorder dco where dco.dateclose > date_last_open and dco.dateclose < date_final and dco.idorder = o.idorder;
			value_return := value_return + current_value;

			total_lines := 0;
		end if;

	end if;

	return  value_return;
END;

$$ LANGUAGE plpgsql;



-- SQL do Victor para me ajudar.
-- create or replace function get_time() RETURNS varchar(8) as $$
-- declare
-- 	time_return varchar(8);
-- begin
-- 	select extract(hour from current_time);
-- $$ language plpgsql;

-- select sum(q1.valuemovement) from 
-- (select idmovement, valuemovement from 
--  movementcash mc where 
--  mc.datemovement >= '2012-08-30' and mc.hourmovement >= '22:26:00' and 
--  (mc.typemovement = 'Abertura' or mc.typemovement = 'Entrada')) q1
-- inner join
-- (select idmovement, valuemovement from 
--  movementcash mc where 
--  mc.datemovement <= '2012-08-31' and mc.hourmovement <= '16:44' and
--  (mc.typemovement = 'Abertura' or mc.typemovement = 'Entrada')) q2
-- on q1.idmovement = q2.idmovement;