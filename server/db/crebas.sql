/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     24.8.2007 13:17:04                           */
/*==============================================================*/


drop table if exists Cena_mereni;

drop table if exists Cenova_skupina;

drop table if exists Dalsi_zpracovani;

drop table if exists Druh_dreva;

drop table if exists Dvakrat;

drop table if exists Loaded;

drop table if exists Manipulace;

drop table if exists Mereni;

drop table if exists Seznam;

drop table if exists Uzivatel;

drop table if exists Zakaznik;

drop table if exists Zakaznik_mereni;

/*==============================================================*/
/* Table: Cena_mereni                                           */
/*==============================================================*/
create table Cena_mereni
(
   IDCena_mereni        bigint not null,
   IDCenova_skupina     bigint,
   IDMereni             bigint,
   primary key (IDCena_mereni)
)
comment = "Tabulka obsahuje seznam cenových skupin pro jednotlivá měření.";

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Cena_mereni
(
   IDCenova_skupina
);

/*==============================================================*/
/* Table: Cenova_skupina                                        */
/*==============================================================*/
create table Cenova_skupina
(
   IDCenova_skupina     bigint not null,
   Popis                char(30),
   Cena                 decimal(8,2),
   m3                   bool default true,
   Globalni             bool default false,
   primary key (IDCenova_skupina)
);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Cenova_skupina
(
   Globalni
);

/*==============================================================*/
/* Table: Dalsi_zpracovani                                      */
/*==============================================================*/
create table Dalsi_zpracovani
(
   IDDalsi_zpracovani   int not null auto_increment,
   Nazev                char(20),
   Cena                 decimal(8,2),
   primary key (IDDalsi_zpracovani)
);

insert into Dalsi_zpracovani(Nazev,Cena) values 
("",0),
("Sámovka",400),
("Pásovka",500);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Dalsi_zpracovani
(
   IDDalsi_zpracovani
);

/*==============================================================*/
/* Table: Druh_dreva                                            */
/*==============================================================*/
create table Druh_dreva
(
   IDDruh_dreva         tinyint not null auto_increment,
   Nazev                char(20),
   P0                   decimal(8,7),
   P1                   decimal(8,7),
   P2                   decimal(8,7),
   Cena                 decimal(8,2),
   primary key (IDDruh_dreva)
);

insert into Druh_dreva(Nazev,P0,P1,P2,Cena) values 
("Smrk",0.57723,0.006968,1.3123,400),
("Borovice - kůra",0.25017,0.0019147,1.7866,500),
("Borovice - borka",1.7015,0.0087620,1.4568,600),
("Buk",-0.040877,0.16634,0.56076,700),
("Dub",1.2474,0.04263,1.0623,800);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Druh_dreva
(
   Nazev
);

/*==============================================================*/
/* Table: Dvakrat                                               */
/*==============================================================*/
create table Dvakrat
(
   IDDvakrat            int not null auto_increment,
   Nazev                char(20),
   Cena                 decimal(8,2),
   primary key (IDDvakrat)
);

insert into Dvakrat(Nazev,Cena) values 
("2 x katrem",400);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Dvakrat
(
   IDDvakrat
);

/*==============================================================*/
/* Table: Loaded                                                */
/*==============================================================*/
create table Loaded
(
   IDLoaded             int not null auto_increment,
   Datetime             datetime,
   Action               enum("load_start","load_stop"),
   primary key (IDLoaded)
)
comment = "Tabulka udává časy, kdy došlo k načtení posledních dat z technologie.";

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Loaded
(
   Datetime
);

/*==============================================================*/
/* Table: Manipulace                                            */
/*==============================================================*/
create table Manipulace
(
   IDManipulace         int not null auto_increment,
   Nazev                char(20),
   Cena                 decimal(8,2),
   primary key (IDManipulace)
);

insert into Manipulace(Nazev,Cena) values 
("Manipulace",400);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Manipulace
(
   IDManipulace
);

/*==============================================================*/
/* Table: Mereni                                                */
/*==============================================================*/
create table Mereni
(
   IDMereni             bigint not null auto_increment,
   IDDruh_dreva         tinyint not null,
   IDZakaznik_mereni    bigint,
   IDManipulace         int,
   IDDalsi_zpracovani   int,
   IDSeznam             int,
   IDDvakrat            int,
   Delka                decimal(4,2),
   Prumer               decimal(3,1),
   Deleted              bit default false,
   Edited               datetime,
   Added                datetime,
   IDTechnolog          int,
   IDZakaznik_technolog int default NULL,
   Status               enum("normal","load","tmp"),
   primary key (IDMereni)
);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Mereni
(
   IDSeznam
);

/*==============================================================*/
/* Table: Seznam                                                */
/*==============================================================*/
create table Seznam
(
   IDSeznam             int not null auto_increment,
   IDZakaznik           int,
   IDCenova_skupina     bigint,
   Archiv               bit default false,
   Datum                date,
   Cena_1               decimal(8,2),
   Cena_2               decimal(8,2),
   Cena_3               decimal(8,2),
   Cena_4               decimal(8,2),
   Cena_5               decimal(8,2),
   Cena_man             decimal(8,2),
   Cena_dalsi_1         decimal(8,2),
   Cena_dalsi_2         decimal(8,2),
   Cena_dvakrat         decimal(8,2),
   Deleted              bit default false,
   Edited               datetime,
   primary key (IDSeznam)
);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Seznam
(
   IDZakaznik
);

/*==============================================================*/
/* Table: Uzivatel                                              */
/*==============================================================*/
create table Uzivatel
(
   IDUzivatel           int not null auto_increment,
   Nick                 char(20),
   Heslo                char(80),
   primary key (IDUzivatel)
);

insert into Uzivatel(Nick,Heslo) 
values ("root",md5("moje.heslo"));

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Uzivatel
(
   Nick
);

/*==============================================================*/
/* Table: Zakaznik                                              */
/*==============================================================*/
create table Zakaznik
(
   IDZakaznik           int not null auto_increment,
   Jmeno                Char(32),
   ICO                  char(9),
   DIC                  char(9),
   Adresa               char(80),
   Telefon              char(16),
   Email                char(30),
   Archiv               bit default false,
   Deleted              bit default false,
   Edited               datetime,
   primary key (IDZakaznik)
);

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Zakaznik
(
   Jmeno
);

/*==============================================================*/
/* Table: Zakaznik_mereni                                       */
/*==============================================================*/
create table Zakaznik_mereni
(
   IDZakaznik_mereni    bigint not null auto_increment,
   Jmeno                char(9),
   Datum                date,
   IDTechnolog          int,
   Added                datetime,
   Status               enum("normal","load","tmp"),
   primary key (IDZakaznik_mereni)
)
comment = "Tabulka zákazníků, pro něž se tento den měřilo. Jsou zde jen základní informace, které pocházejí z měřícího systému. Zákazník je identifikován pouze na základě osmiznakového jména. Po načtení informací z měřícího systému do databáze určí obsluha, který zákazník z databáze (normálních) zákazníků odpovídá tomuto.";

/*==============================================================*/
/* Index: Index_1                                               */
/*==============================================================*/
create index Index_1 on Zakaznik_mereni
(
   Jmeno
);

alter table Cena_mereni add constraint FK_Reference_16 foreign key (IDCenova_skupina)
      references Cenova_skupina (IDCenova_skupina) on delete restrict on update restrict;

alter table Cena_mereni add constraint FK_Reference_17 foreign key (IDMereni)
      references Mereni (IDMereni) on delete restrict on update restrict;

alter table Mereni add constraint FK_Reference_1 foreign key (IDSeznam)
      references Seznam (IDSeznam) on delete cascade on update restrict;

alter table Mereni add constraint FK_Reference_14 foreign key (IDDvakrat)
      references Dvakrat (IDDvakrat) on delete set null on update restrict;

alter table Mereni add constraint FK_Reference_3 foreign key (IDDruh_dreva)
      references Druh_dreva (IDDruh_dreva) on delete restrict on update restrict;

alter table Mereni add constraint FK_Reference_7 foreign key (IDZakaznik_mereni)
      references Zakaznik_mereni (IDZakaznik_mereni) on delete set null on update restrict;

alter table Mereni add constraint FK_Reference_8 foreign key (IDManipulace)
      references Manipulace (IDManipulace) on delete set null on update restrict;

alter table Mereni add constraint FK_Reference_9 foreign key (IDDalsi_zpracovani)
      references Dalsi_zpracovani (IDDalsi_zpracovani) on delete set null on update restrict;

alter table Seznam add constraint FK_Reference_15 foreign key (IDCenova_skupina)
      references Cenova_skupina (IDCenova_skupina) on delete restrict on update restrict;

alter table Seznam add constraint FK_Reference_5 foreign key (IDZakaznik)
      references Zakaznik (IDZakaznik) on delete cascade on update restrict;

