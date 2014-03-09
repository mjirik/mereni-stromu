<?php
// $Id: seznamy.php 100 2007-08-20 18:27:25Z Mira $
/**
 * @file seznamy.php
 * @brief 
 * Soubor zajišťuje vypisování zakázek zákazníků.
 * @todo prověřit nezbytnost 
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}
include ('./class/template.class.php');
include ('./inc/funkce.inc.php');
$link = connect_db();
$body = & new Template('./tmpl/seznamy.tmpl.php');
$all = & new Template('./tmpl/all.tmpl.php');
$vysledek = seznamy_zakaznika($_GET["id"],$link);
if ($vysledek == 0){
  $zaznamy["echo"] = "Neexistuje žádná zakázka";
}
else{
  $zaznamy = $vysledek;
} 

$body->set('zzaznamy', $zaznamy);
$all->set('body', $body);
echo $all->fetch();

?>

