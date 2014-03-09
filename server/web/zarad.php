<?php
//$Id: zarad.php 90 2007-08-18 19:27:18Z Mira $
/**
 * @file zarad.php
 * Skript podle vstupních proměnných GET přiřazuje záznamy do seznamů atd.
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}
include ('./class/template.class.php');
include ('./inc/funkce.inc.php');
$link = connect_db(); 

$zobrazit = true;

$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/zarad.tmpl.php');

$link = connect_db();
if (!empty($_GET["sIDSeznam"])){
  $all->set('nadpis', "Přesun zakázky"); 
  if (!empty($_GET["IDZakaznik"])){
    //TODO stored procedure update
    $param = parametry_seznamu($_GET["sIDSeznam"],$link);
    seznamChZakaznik($_GET["sIDSeznam"],$_GET["IDZakaznik"],$link);
//     mysql_query_e(
//     "call sp_SeznamChZakaznik(".$_GET["sIDSeznam"].",".$_GET["IDZakaznik"].")"
//     ,$link);
    $zobrazit = false;
    //$jmeno = $param["Jmeno"];
    $_GET["id"]=$param["IDZakaznik"];
    include("zakazky.php");
  }
  else{
    $param = parametry_seznamu($_GET["sIDSeznam"],$link);
    $jmeno = $param["Jmeno"];
    $nazev = $_GET["sIDSeznam"]." : ".sql_datum($param["Datum"]);
    $szaznamy = seznam_mereni($_GET["sIDSeznam"],$link);
    $dzakaznici = zakaznici($link);
    $src="sIDSeznam=".$_GET["sIDSeznam"];
    //$href = link($zaznam["Jmeno"],"zarad.php?$src&amp;IDZakaznik=".$zaznam["IDZakaznik"]);
  }
}

if (!empty($_GET["sIDMereni"])){
  $all->set('nadpis', "Přesun měření"); 
  if (!empty($_GET["IDSeznam"])){
    $mereni = get_mereni($_GET["sIDMereni"],$link);
    mereniChSeznam($_GET["sIDMereni"],$_GET["IDSeznam"],$link);
    $_GET = null;
    $_GET["id"] = $mereni["IDSeznam"];
    include("edit_seznam.php");
    $zobrazit = false;
  }
  else{
    if (!empty($_GET["IDZakaznik"])){
      $dseznamy = seznamy_zakaznika($_GET["IDZakaznik"],$link);
      $src="sIDMereni=".$_GET["sIDMereni"]."&amp;IDZakaznik=".$_GET["IDZakaznik"];
    }
    else{
      $src="sIDMereni=".$_GET["sIDMereni"];
    }
    $mereni = get_mereni($_GET["sIDMereni"],$link);
    $szaznamy = seznam_mereni($mereni["IDSeznam"],$link);
    $param = parametry_seznamu($mereni["IDSeznam"],$link);
    $jmeno = $param["Jmeno"];
    $nazev = $mereni["IDSeznam"]." : ".sql_datum($param["Datum"]);
    $dzakaznici = zakaznici($link);
    
    //$href = link($zaznam["Jmeno"],"zarad.php?$src&amp;IDZakaznik=".$zaznam["IDZakaznik"]);
  }
}

// Jméno je buď jméno zákazníka, nebo jméno Zakaznik_mereni
// nazev je název seznamu (zakázky), nebo datum vytvoření Zakaznik_mereni
// Záznamy jsou příslušná měření.
if ($zobrazit){
  
  $body->set('jmeno', $jmeno);
  $body->set('nazev', $nazev);
  $body->set('szaznamy', $szaznamy);
  $body->set('dzakaznici', $dzakaznici);
  $body->set('src', $src);
  $body->set('dseznamy', $dseznamy);
  $body->set('IDMereni', $_GET["sIDMereni"]);
  $body->set('confDalsiZpracovani', $confDalsiZpracovani);
  
  //$body->set('href', $href);

  $all->set('body', $body);

  echo $all->fetch();
}
?>
