<?php
// $Id: edit_zakaznik.php 73 2007-08-12 14:35:42Z Mira $
/**
 * @file edit_zakaznik.php
 * Editace zákaníka.  
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}


include ('./class/template.class.php');
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/edit_zakaznik.tmpl.php');


//include ("./connect_info.inc.php");
include ('./inc/funkce.inc.php');
mysql_connect(SQL_HOST, SQL_USERNAME, SQL_PASSWORD);
mysql_select_db(SQL_DBNAME);

if (!is_numeric ($_GET["id"])){
  // vyskytne se pokud se někdo pokusí do adresy připsat něco pitomýho
  echo "Chyba při předávání identifikátoru!";
}
else{
  if (empty($_POST)){ //pokud nemáme data z POSTu, načteme je z databáze. Pak už je předáváme v postu.
    $vysledek = mysql_query("select Jmeno,ICO,DIC,Adresa,Telefon,Email from Zakaznik where IDZakaznik = ".$_GET["id"].";")
    or die("Nelze vykonat definicní dotaz: " . mysql_error());
    if (mysql_num_rows($vysledek)== 0){
      echo ("Nenalezen odpovídající řádek!");
      $BudemeZobrazovat=false;
    }
    else{
      $zaznam=MySQL_Fetch_Array($vysledek);
      $BudemeZobrazovat=true;
    }
  }
  else{
    //echo "POST";
    $zaznam["Jmeno"]=$_POST["jmeno"];
    $zaznam["Adresa"]=$_POST["adresa"];
    $zaznam["ICO"]=$_POST["ico"];
    $zaznam["DIC"]=$_POST["dic"];
    $zaznam["Telefon"]=$_POST["telefon"];
    $zaznam["Email"]=$_POST["email"];

    $BudemeZobrazovat=false;
    // kontroly
    if ((strlen(trim($_POST["jmeno"])) < 3)){
      $echo[] = "Jméno musí být alespoň tři znaky dlouhé!";
      $BudemeZobrazovat=true;
    }
    if ((strlen($_POST["email"])>0)&(!jeEmail($_POST["email"]))){
      $echo[] = "Nesprávný tvar emailu!";
      $BudemeZobrazovat=true;
    }
    // tady je výkonná část. Uživatel kliknul na Uložit! a nebyl žádný problém.
    if (!$BudemeZobrazovat){
      //echo "Zpracovávám ".$_POST["jmeno"];  
      mysql_query("update Zakaznik set ".
        "Jmeno = \"".$_POST["jmeno"]."\" ".
        ",ICO = \"".$_POST["ico"]."\" ".
        ",DIC = \"".$_POST["dic"]."\" ".
        ",Adresa = \"".$_POST["adresa"]."\" ".
        ",Telefon = \"".$_POST["telefon"]."\" ".
        ",Email = \"".$_POST["email"]."\" where IDZakaznik = ".$_GET["id"].";")
        or die("Nelze vykonat definicní dotaz: " . mysql_error());
        include("zakazky.php");
      
    }
  }

  
  // vykreslení formuláře
  if ($BudemeZobrazovat){  
//     $zaznam=MySQL_Fetch_Array($vysledek);
    
    $body->set('zakaznik', $zaznam);
    //$body->set('id', $_GET["id"]);
    $body->set('action',"edit_zakaznik.php?id=".$_GET["id"]);
    $body->set('samostatne', true);
    $body->set('echo', $echo);
    $all->set('body', $body);
    $all->set('nadpis',$zaznam["Jmeno"]);

    echo $all->fetch();

  }
}

 

?>
