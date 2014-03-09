<?php
// $Id: load_file.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file load_file.php
 * Zajištění načítání ze souboru.
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}
include ('./class/template.class.php');
include ('./inc/funkce.inc.php');
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/load_file.tmpl.php');
// ahoj.
// pridavam kometar
$link = connect_db();
if ($_GET["start"]==true){
  load_mereni_from_files("./db/zakaznici.dat","./db/mereni.dat",$link);
}
$body->set('echo', $echo);

$all->set('body', $body);

echo $all->fetch();
?>
