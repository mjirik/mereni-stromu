/* uložené procedury do projekty Pily */
/* do databáze se nahrávají */
/* source c:\wamp\www\Pila\db\sp.sql */
use Pila;

/* insert, update*/
drop procedure if exists sp_deleteMereni;
drop procedure if exists sp_deleteSeznam;
drop procedure if exists sp_deleteZakaznik;
drop procedure if exists sp_insertZakaznik;
drop procedure if exists sp_SeznamInArchiv;
drop procedure if exists sp_ZakaznikInArchiv;
drop procedure if exists sp_SeznamOutArchiv;
drop procedure if exists sp_ZakaznikOutArchiv;
drop procedure if exists sp_SeznamChZakaznik;
drop procedure if exists sp_MereniChSeznam;
drop procedure if exists sp_insertLoadZakaznik;
drop procedure if exists sp_Timemark;
drop procedure if exists sp_insertLoadMereni;
drop procedure if exists sp_Load;

/* select */
drop procedure if exists sp_volnaMereni;
drop procedure if exists sp_seznamMereni;

delimiter $$

/* insert, update */
/*----------------*/

create procedure sp_deleteMereni(radek bigint)
begin
  delete from Mereni where IDMereni = radek;
/*
  update Mereni
  set Deleted = true where IDMereni = radek;
*/
end $$

/**
 * Procedura slouží k odstranìní zakázky.
 * Pùvodnì byly záznamy oznaèovány jen parametrem deleted, na pøání zákazníka 
 * jsou však mazány nevratnì.   
 */
create procedure sp_deleteSeznam(radek bigint)
begin
  delete from Seznam where IDSeznam = radek;
/*
  update Seznam left join Mereni
  on 
  Seznam.IDSeznam = Mereni.IDSeznam
  set 
  Mereni.Deleted = true,
  Seznam.Deleted = true
  where Seznam.IDSeznam = radek;
*/
end $$

create procedure sp_deleteZakaznik(radek bigint)
begin
  delete from Zakaznik where IDZakaznik = radek;
/*  update Seznam left join Mereni
  on Seznam.IDSeznam = Mereni.IDSeznam
  right join Zakaznik
  on Seznam.IDZakaznik = Zakaznik.IDZakaznik
  set 
  Mereni.Deleted = true,
  Seznam.Deleted = true,
  Zakaznik.Deleted = true
  where Zakaznik.IDZakaznik = radek;
*/
end $$

/*procedura vkládání zákazníka*/
create procedure sp_insertZakaznik(jmeno char(32),ico char(9), dic char(9), adresa char(80),telefon char (16),email char(30))
begin
  insert into 
  Zakaznik (Jmeno,ICO,DIC,Adresa,Telefon,Email) 
  values (jmeno,ico,dic,adresa,telefon,email);
  
  /*select last_insert_id() as ID;*/
end $$

create procedure sp_SeznamInArchiv(radek int)
begin
update Seznam set Archiv = true where IDSeznam = radek;
end $$

create procedure sp_ZakaznikInArchiv(radek int)
begin
update Zakaznik set Archiv = true where IDZakaznik = radek;
end $$

create procedure sp_SeznamOutArchiv(radek int)
begin
update Seznam set Archiv = false where IDSeznam = radek;
end $$

create procedure sp_ZakaznikOutArchiv(radek int)
begin
update Zakaznik set Archiv = false where IDZakaznik = radek;
end $$

/*Vrátí seznam nepøiøazených mìøení*/
create procedure sp_volnaMereni()
begin
  select 
  Zakaznik_mereni.Datum as Datum, Zakaznik_mereni.IDZakaznik_mereni as IDZakaznik_mereni, 
  Jmeno, count(Mereni.IDZakaznik_mereni) as Pocet_stromu
  from 
  Zakaznik_mereni left join Mereni 
  on Zakaznik_mereni.IDZakaznik_mereni = Mereni.IDZakaznik_mereni 
  where  
  Mereni.IDSeznam is NULL 
  group by Zakaznik_mereni.IDZakaznik_mereni
  order by Datum, Jmeno
  ;
end $$


create procedure sp_seznamMereni(radek bigint)
begin
select 
  Mereni.IDMereni as IDMereni,
  Mereni.Delka as Delka, 
  Mereni.Prumer as Prumer, 
  Mereni.IDDruh_dreva as IDDruh_dreva,
  Druh_dreva.Nazev as Druh_dreva, 
  Druh_dreva.P0 as P0,
  Druh_dreva.P1 as P1,
  Druh_dreva.P2 as P2,
  Mereni.IDManipulace as IDManipulace, 
  Mereni.IDDalsi_zpracovani as IDDalsi_zpracovani,
  Dalsi_zpracovani.Nazev as Dalsi_zpracovani
  from 
  Seznam 
  join Mereni on Seznam.IDSeznam = Mereni.IDSeznam 
  join Druh_dreva on Mereni.IDDruh_dreva = Druh_dreva.IDDruh_dreva
  left join Dalsi_zpracovani 
  on 
  Dalsi_zpracovani.IDDalsi_zpracovani = Mereni.IDDalsi_zpracovani  
  where 
  (Seznam.IDSeznam = radek) and (Mereni.Deleted <> true) 
  order by Mereni.IDDruh_dreva;
end $$

create procedure sp_SeznamChZakaznik(seznam int, zakaznik int)
begin
  update Seznam set IDZakaznik=zakaznik where IDSeznam = seznam;
end $$


create procedure sp_MereniChSeznam(mereni bigint, seznam int)
begin
  update Mereni set IDSeznam=seznam where IDMereni = mereni;
end $$


/*procedura vkládání zákazníka*/
create procedure sp_insertLoadZakaznik(id int, jmeno char(32))
begin
  insert into 
  Zakaznik_mereni (IDTechnolog, Jmeno, Datum, Added,Status) 
  values (id, jmeno, now(),now(),"load");
  
  /*select last_insert_id() as ID;*/
end $$

create procedure sp_insertLoadMereni(
  idzak int, 
  delka decimal(4,2), 
  prumer decimal(3,1), 
  druh_dreva int, 
  manipulace int, 
  dalsi_zpracovani int, 
  dvakrat int )
begin

  insert into 
  Mereni(
    IDZakaznik_technolog, 
    IDDruh_dreva, 
    IDManipulace,
    IDDalsi_zpracovani,
    IDDvakrat,
    Delka,
    Prumer,
    Added,
    Status,
    IDZakaznik_mereni)
  values (
    idzak,
    druh_dreva,
    manipulace,
    dalsi_zpracovani,
    dvakrat,
    delka,
    prumer,
    now(),
    "load",
    (select
       IDZakaznik_mereni
     from 
       Zakaznik_mereni 
     where 
       (IDTechnolog = idzak) 
       and 
       (Added >= (select max(Datetime) from Loaded))
       and (Status = "load")
     order by Added desc 
     limit 1))
  ;
end $$


create procedure sp_Timemark(act enum("load_start","load_stop"))
begin
  insert into
  Loaded(Datetime,Action)
	
  values (now(),act);
end $$

/**
 * Procedura provede donaètení záznamù do databáze. 
 */
create procedure sp_Load()
begin
  update Zakaznik_mereni 
  set 
    Status = "normal" 
  where 
    (Added >= (select max(Datetime) from Loaded)) and (Status = "load");
  update Mereni
  set 
    Status = "normal" 
  where 
    (Added >= (select max(Datetime) from Loaded)) and (Status = "load");

  call sp_Timemark("load_stop");
end $$


create procedure sp_clear_db()
begin
/*  delete from Zakaznik_mereni where  */
end $$



delimiter ;

