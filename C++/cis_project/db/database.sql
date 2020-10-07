create table criteria
(
  id                serial not null
    constraint criteria_pkey
    primary key,
  price             double precision,
  quantity_in_stock integer,
  delivery_time     integer,
  number_of_clients integer,
  provider_lifetime integer
);

alter table criteria
  owner to cis;

create table providers
(
  id                serial  not null
    constraint supplier_details_pkey
    primary key,
  name_organization text    not null,
  address           text    not null,
  phone             integer,
  country           text    not null,
  criteria          integer not null
    constraint fk_supplier_details__criteria
    references criteria
);

alter table providers
  owner to cis;

create index idx_supplier_details__criteria
  on providers (criteria);



INSERT into criteria values (1,130,10,7,3000,5);
INSERT into supplier_details values (1, 'Врим крафт','Москва, Ленинский проспект 45-3',576895,'Россия',1);
INSERT into criteria values (2,170,19,5,40000,10);
INSERT into supplier_details values (2, 'Bakus','Пенза, ул. Овощная 40 ',2505506,'Россия',2);
INSERT into criteria values (3,113,5,6,2375,4);
INSERT into supplier_details values (3, 'Самбир','Самара, ул. Гагарина, д. 60',846300405,'Россия',3);
INSERT into criteria values (4,110,7,14,1560,1);
INSERT into supplier_details values (4, 'Русский пивовар','Владимир, Пригородная улица, 1А',9402884,'Россия',4);
INSERT into criteria values (5,160,11,14,20000,8);
INSERT into supplier_details values (5, 'Новопермский пивовар','Пермь, Попова, 25/1',5510705,'Россия',5);
INSERT into criteria values (6,115,4,7,126321,11);
INSERT into supplier_details values (6, 'Beermachines','Казань, ул.Х.Мавлютова 17Б-50',3460337,'Россия',6);
INSERT into criteria values (7,120,7,5,144000,10);
INSERT into supplier_details values (7, 'МирБир','Москва,ул. Измайловский вал, 30-3',6655086,'Россия',7);
INSERT into criteria values (8,285,1,7,264899,14);
INSERT into supplier_details values (8,'Pivovarnya','Москва, Огородный проезд 20-2',4096269,'Россия',8);
INSERT into criteria values (9,110,5,10,15142,3);
INSERT into supplier_details values (9,'AlexBriwer','Воронеж, ул. Войкова',8582882,'Россия',9);
INSERT into criteria values (10,115,3,9,25400,4);
INSERT into supplier_details values (10,'СамоВар','Липецк, ул.Гагарина 34',242127,'Россия',10);

INSERT into criteria values (11,92,4,7,9400,3);
INSERT into supplier_details values (11,'Солод+','Липецк, ул. Интернациональная,42',5952233,'Россия',11);
INSERT into criteria values (12,147,10,5,140576,7);
INSERT into supplier_details values (12,'Housebeer','Санкт-ПетербургБольшой проспект Петроградской стороны 100 ',634535,'Россия',12);
INSERT into criteria values (13,150,15,2,13278,4);
INSERT into supplier_details values (13,'Brewtime','Пенза, ул. Суворова,111 А',535554,'Россия',13);
INSERT into criteria values (14,87,10,14,25400,3);
INSERT into supplier_details values (14,'Solodok','Киев, ул. Дегтяревская 33а',0443537218,'Украина',14);
INSERT into criteria values (15,170,6,8,17896,3);
INSERT into supplier_details values (15,'Градус','Рязань, ул. Грибоедова 41',514901,'Россия',15);
INSERT into criteria values (16,170,1,21,1173453,11);
INSERT into supplier_details values (16,'100sp','Владивосток, a/я 46',578963,'Россия',16);
INSERT into criteria values (17,165,7,7,18356,4);
INSERT into supplier_details values (17,'Hop and Malt','-',578963,'USA',17);
INSERT into criteria values (18,100,4,20,8765,4);
INSERT into supplier_details values (18,'PromUA','Киев, Харьковское шоссе, 201/203',937240,'Украина',18);
INSERT into criteria values (19,142,15,7,10786,3);
INSERT into supplier_details values (19,'Prosolod','Санкт-Петербург,Железнодорожный пр 36',608711,'Россия',19);
INSERT into criteria values (20,300,13,10,45632,6);
INSERT into supplier_details values (20,'МирБирUA','Днепр, ул. Шмидта 8',559945,'Украина',20);