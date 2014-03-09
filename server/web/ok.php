<?php
// $Id: ok.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file ok.php
 * Skript vykresluje hlášku s potvrzovacím a stornovacím tlačítkem. 
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}
session_start();
//if ($_SESSION[]
include ('./class/template.class.php');
include ('./inc/funkce.inc.php');
//$link = connect_db();

$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/ok.tmpl.php');

$body->set('text', $_SESSION["ok_text"].$_GET["ok_text"]);
$body->set('hrefOk', $_SESSION["ok_hrefOk"].$_GET["ok_hrefOk"]);
$body->set('hrefStorno', $_SESSION["ok_hrefStorno"].$_GET["ok_hrefStorno"]);
$all->set('body', $body);
echo $all->fetch();
?>
