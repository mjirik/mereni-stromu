<?php
// $Id: zakaznici_mereni.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file zakaznici_mereni.php
 * @todo Zjisti k čemu je a zda je skript využíván. 
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}

include_once ('./inc/funkce.inc.php');
include_once ('./class/template.class.php');
include_once ('./class/kalendar.class.php');
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/stat.tmpl.php');

$link = connect_db();

$zobrazit = true;

if ($zobrazit){
  $zaznamy = vsechnaMereni($link);
  $body->set('zaznamy', $zaznamy);
  $all->set('body', $body);
  $all->set('nadpis', "Statistiky");
  echo $all->fetch();
}
?>
