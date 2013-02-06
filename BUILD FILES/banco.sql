create table if not exists client(
	idClient integer not null primary key,
	name varchar(60) not null,
	phone varchar(14),
	street varchar(50), nhouse varchar(5), complement varchar(50), district varchar(30), cep varchar(9),
	shipping float,
	note text
);

create table if not exists clientBusiness(
	idClient integer not null primary key,
	nameRespondible varchar(60) not null,
	corporateName varchar(150),
	fantasyName varchar(100),
	cnpj varchar(25),
	phone varchar(14),
	street varchar(50), nhouse varchar(5), complement varchar(50),
	district varchar(30), cep varchar(9),
	shipping float,
	note text
);

create table if not exists configAlert(
	daysAlertOrder integer
);

create table if not exists generalConfig(
	dateLastBackup date
);

/*create table if not exists provider(
	idProvider integer not null primary key,
	name varchar(30) not null,
	phone varchar(9), branch varchar(5),
	phone2 varchar(9), branch2 varchar(5),
	phone3 varchar(9), branch3 varchar(5),
	nextel varchar(9),
	cpf varchar(20), rg varchar(12),
	namecontact varchar(30),
	site varchar(50), email varchar(50),
	cc1 varchar(10), ag1 varchar(10), bank1 varchar(10),
	cc2 varchar(10), ag2 varchar(10), bank2 varchar(10),
	street varchar(50), district varchar(30), nhouse varchar(5), complement varchar(10), cep varchar(9),
	note text
);*/

create table if not exists employee(
	idEmployee integer not null primary key,
	name varchar(30) not null,
	phone varchar(14), phone2 varchar(14), nextel varchar(9),
	street varchar(50), nhouse varchar(5), complement varchar(20),
	district varchar(30), cep varchar(9),
	jobrole varchar(30), commission float,
	cpf varchar(20), rg varchar(12),
	cc1 varchar(10), ag1 varchar(10), bank1 varchar(40),
	note text
);

create table if not exists statusEmployee(
	idEmployee integer not null,
	status varchar(12),
	primary key(idEmployee, status),
	foreign key(idEmployee) references employee(idEmployee)
);

create table if not exists category(
	idCategory integer not null primary key,
	name varchar(30)
);

create table if not exists subcategory(
	idCategory integer not null,
	idSubCategory integer not null,
	primary key(idCategory, idSubCategory),
	name varchar(30)
);

create table if not exists product(
	idProduct integer not null primary key,
	name varchar(60) not null,
	nickname varchar(60),
	idCategory integer,
	idSubCategory integer,
	note text,
	foreign key(idCategory) references category(idCategory)
);

create table if not exists size(
	idSize integer not null primary key,
	name varchar(20)
);

create table if not exists productValue(
	idProduct integer not null,
	idSize integer not null,
	valueProduct float, /* só value é palavra reservada*/
	primary key(idProduct, idSize),
	foreign key(idProduct) references product(idProduct),
	foreign key(idSize) references size(idSize)
);

create table if not exists bill(
	idBill integer not null primary key,
	name varchar(30),
	valueBill float,
	typeBill varchar(15),
	status varchar(10),
	dateExpiration date,
	datePayment date,
	note text
);

create table if not exists movementCash(
	idMovement integer not null primary key,
	valueMovement float,
	typeMovement varchar(10),
	dateMovement date,
	hourMovement time,
	note text
);

-- tabelas de pedido
create table if not exists ordered( -- ver tradução disso ai, order é palavra reservada
	idOrder integer not null primary key,
	typeClient varchar(1), -- n para normal, b para business
	idClient integer, -- não é obrigatório, se for balcão não necessariamente tem client
	dateOrder date,
	timeOrder time,
	typeOrder varchar(10), -- (entrega, balcão, mesa)
	valueOrder float,
	valueShipping float,
	valuePaid float,
	isPaid varchar(5),
	statusOrder varchar(25), -- (entregue, atrasado, etc)
	numberOrder varchar(10)
);


create table if not exists employeeorder(
	idorder integer not null,
	idemployee integer not null,
	primary key(idorder, idemployee),
	foreign key(idorder) references ordered(idorder),
	foreign key(idemployee) references employee(idemployee)
);

create table if not exists itemOrder(
	idOrder integer not null,
	idItem integer not null,
	idSize integer,
	valueitem float,
	quantity float, -- ver tradução tbm
	note text,
	primary key(idOrder, idItem),
	foreign key(idOrder) references ordered(idOrder),
	foreign key(idSize) references size(idSize)
);

create table if not exists productItem(
	idOrder integer not null,
	idItem integer not null,
	idProduct integer not null, -- não botarei idproduto com chave estrangeira, pq produto misto terá id = 0, que não ta cadastrado
	part integer not null
);

create table if not exists paymentOrder(
	idOrder integer not null,
	paymentForm varchar(30),
	paymentValue float,
	foreign key(idOrder) references ordered(idOrder)
);

create table if not exists alert(
	idAlert integer not null primary key,
	title varchar(80),
	subject varchar(50),
	dateEvent varchar(10),
	hourEvent varchar(5),
	dateAlert varchar(10),
	hourAlert varchar(5),
	priority varchar(10),
	alerted integer,
	note text
);

create table if not exists schedule(
	idOrder integer not null,
	dateSchedule varchar(10),
	hourSchedule varchar(5),
	foreign key(idOrder) references ordered(idOrder)
);

create table if not exists scheduleAlert(
	idOrder integer not null,
	idAlert integer not null,
	primary key(idOrder, idAlert),
	foreign key(idOrder) references ordered(idOrder),
	foreign key(idAlert) references alert(idAlert)
);

create table if not exists taskSimple(
	idTask integer not null primary key,
	dateTask date,
	task text
);

create table if not exists business(
	name varchar(50),
	phone varchar(14),
	address varchar(100),
	district varchar(30),
	city varchar(30),
	stateBusiness varchar(30)
);

create table if not exists configOrder(
	numberofcopies integer,
	fontSize varchar(1), /* p, m ou g*/
	widthpaper integer,
	typenumbering varchar(1),
	currentnumber varchar(10)
);

create table if not exists paymenttypes(
	idpayment integer not null primary key,
	paymentform varchar(30)
);

create table if not exists datecloseorder(
	idorder integer not null primary key,
	dateclose date,
	hourclose time
);

create table if not exists usersystem(
	idusersystem integer not null primary key,
	username varchar(30),
	password varchar(100),
	secretquestion varchar(150),
	secretanswer varchar(600)
);

create table if not exists accesslevel(
	iduser integer primary key,
	cantreatment boolean,
	canclient boolean,
	canproduct boolean,
	canordered boolean,
	canemployee boolean,
	canalert boolean,
	canbill boolean,
	cancash boolean,
	cancreateuser boolean,
	canalteraccess boolean,
	foreign key (iduser) references usersystem(idusersystem)
);

create table if not exists buyaccess(
	code varchar(100)
);

create table if not exists statusServer(
	status varchar(30)
);

create table if not exists treatmentTable(
	idorder integer not null,
	ismedia boolean,
	iscompound boolean,
	ischildren boolean,
	isfather boolean,
	idbartender integer,
	linkedtables varchar(350),
	countpeople integer,
	currenttable integer
);

--insert into paymenttypes values(1, 'Dinheiro'),(2, 'Cartão de crédito'),(3, 'Cartão de débito');

/*create table if not exists putStock(
	idPutStock integer not null primary key,
	dateOrder date,
	idProvider integer not null,
	orderValue float,
	discountValue float,
	shippingValue float,
	otherValues float,
	dateDelivery date,
	foreign key(idProvider) references provider(idProvider)
);

create table if not exists paymentPutStock(
	idPutStock integer not null,
	idPayment integer not null,
	formPayment varchar(30),
	valuePayment float,
	primary key(idPutStock, idPayment),
	foreign key(idPutStock) references putStock(idPutStock)
);

create table if not exists productPutStock(
	idPutStock integer not null,
	idProduct integer not null,
	quantity float,
	valueUnitary float,
	primary key(idPutStock, idProduct),
	foreign key(idProduct) references product(idProduct)
);*/

-- proceduresS