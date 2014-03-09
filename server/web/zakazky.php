<?php
// $Id: zakazky.php 100 2007-08-20 18:27:25Z Mira $
/**
 * @file zakazky.php
 * Skript zajišťuje výpis zakázek. 
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}
//print_r($_SESSION);
include_once ('./class/template.class.php');
include_once ('./inc/funkce.inc.php');
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/zakazky.tmpl.php');
$szn = & new Template('./tmpl/seznamy.tmpl.php');
session_start();
$_SESSION["page"] = "zakazky.php";
$_SESSION["ok_text"]="Opravdu chcete řádek smazat?";

$link = connect_db(); 
include_once("./inc/mysql.inc.php");

//výmaz zákazníka
if (!empty($_GET["delid"])){
  deleteZakaznik($_GET["delid"],$link);
}

if (!empty($_GET["delSezID"])){
  deleteSeznam($_GET["delSezID"],$link);
}

if (!empty($_GET["archZakID"])){
  if ($_GET["archZakID"] > 0)
    zakaznikInArchiv($_GET["archZakID"],$link);
  else 
    zakaznikOutArchiv(-$_GET["archZakID"],$link);
}

if (!empty($_GET["archSezID"])){
  if ($_GET["archSezID"] > 0)
    seznamInArchiv($_GET["archSezID"],$link);
  else 
    seznamOutArchiv(-$_GET["archSezID"],$link);
}

if (!empty($_GET["insertSeznam"])){ 
  novySeznam($_GET["insertSeznam"],$link);
}


if ($_GET["archivSeznam"]==-1){
  $_SESSION["archivSeznam"]=0;
}
if ($_GET["archivSeznam"]==1){
  $_SESSION["archivSeznam"]=1;
}

if ($_GET["archivZakaznik"]==-1){
  $_SESSION["archivZakaznik"]=0;
}
if ($_GET["archivZakaznik"]==1){
  $_SESSION["archivZakaznik"]=1;
}


$zaznamy = zakaznici($link,$_SESSION["archivZakaznik"]);
if(!empty($_GET["id"])){
  $_SESSION["page"] = "zakazky.php?id=".$_GET["id"];
  $zzaznamy = seznamy_zakaznika($_GET["id"],$link,$_SESSION["archivSeznam"]);
  if ($zzaznamy == null){
    $zzaznamy["echo"] = "Nenalezen žádný záznam";
  }
  
  foreach ($zaznamy as $zaznam){
    if ($_GET["id"] == $zaznam["IDZakaznik"]){
      $jmeno = $zaznam["Jmeno"];
    }
  }

  $szn->set('zzaznamy', $zzaznamy);
  $szn->set('href', "edit_seznam.php?id=");
  
  $seznamuVArchivu = pocet_seznamu_v_archivu($_GET["id"],$link);
  
}
$zakaznikuVArchivu = pocet_zakazniku_v_archivu($link);


$body->set('zaznamy', $zaznamy);
$body->set('seznamuVArchivu', $seznamuVArchivu);
$body->set('zakaznikuVArchivu', $zakaznikuVArchivu);
$body->set('id', $_GET["id"]);
$body->set('jmeno', $jmeno);
$body->set('sez_div', $szn);

$all->set('nadpis',"Zakázky");
$all->set('nadpis2',$jmeno);
$all->set('body', $body);

echo $all->fetch();
?>
