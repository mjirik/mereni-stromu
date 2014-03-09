<?php
// $Id: zakaznik_seznam.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file zakaznik_seznam.php
 * @todo Zjistit využívanost skriptu. 
 */
# podle toho co dostane v get, přiřazuje buď všechny záznamy od Zakaznik_mereni, nebo 

include ('./class/template.class.php');
include ('./inc/funkce.inc.php');
$link = connect_db();
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/zakaznik_seznam.tmpl.php');
$seznamy = & new Template('./tmpl/seznamy.tmpl.php');
$zakaznici = & new Template('./tmpl/zakaznici.tmpl.php');

$vykreslujeme = true;
if (!empty($_GET["idsez"])){
  //if (!empty($_GET)

}

if ($vykreslujeme){
  if (!empty($_GET["idzakmer"])){
    $coprirazujeme = "Zákazník_měření : ".
    jmeno_zakaznik_mereni($_GET["idzakmer"],$link);
  }
  if (!empty($_GET["idmer"])){
    $coprirazujeme = "Měření : ";
  }
  
  if (!empty($_GET["idzak"])){
    $pole_seznamy_ = seznamy_zakaznika($_GET["idzak"],$link);
    if ($pole_seznamy_ == 0){
      $pole_seznamy["echo"] = "Nenelezen žádný seznam";
    }
    else{
      $pole_seznamy= $pole_seznamy_;
    }
    $href_seznamy = "zakaznik_seznam.php?idzak=".$_GET["idzak"]."&idsez=";
    $idzak = $_GET["idzak"];
  }
  
  $pole_zakaznici = zakaznici($link);
  if ($pole_zakaznici == 0){
    $echo_zakaznici = "Nebyl nalezen žádný zákazník";
  }
  $href_zakaznici = "zakaznik_seznam.php?idzak=";
 
  $seznamy->set('zzaznamy', $pole_seznamy);
  $seznamy->set('href', $href_seznamy);
  $zakaznici->set('href', $href_zakaznici);
  $zakaznici->set('id', $idzak);
  $zakaznici->set('zaznamy', $pole_zakaznici);
  $zakaznici->set('echo', $echo_zakaznici);
  $body->set('coprirazujeme', $coprirazujeme);
  $body->set('zakaznici', $zakaznici);
  $body->set('seznamy', $seznamy);
  $all->set('body', $body);
  echo $all->fetch();

}
// $vysledek = mysql_query("select IDSeznam,Datum from Seznam where IDZakaznik = ".$_GET["id"].";")
// or die("Nelze vykonat definicní dotaz: " . mysql_error());
// if (mysql_num_rows($vysledek)== 0){
//   die("Nenalezen žádný odpovídající řádek!");
// }
// else{
//   while ($zaznam=MySQL_Fetch_Array($vysledek)){
//     $zaznamy[]=$zaznam;
//   }
//   $bbody->set('zaznamy', $zaznamy);//    }
// };
?>

