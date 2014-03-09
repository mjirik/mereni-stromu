<?php
// $Id: new_seznam.php 58 2007-08-03 10:29:44Z Mira $
//include ('./class/template.class.php');

//$all = & new Template('./tmpl/all.tmpl.php');
//$body = & new Template('./tmpl/edit_seznam.tmpl.php');
session_start();
$next = $_SESSION["page"];
$_SESSION["page"]=null;

include_once ("connect_info.php");
include_once ('./inc/funkce.inc.php');
$link = connect_db();
//TODO if close_older_seznam

if (!empty($_GET["IDZakaznik"])){
  
  novySeznam($_GET["IDZakaznik"],$link);
}
if (!empty($next)){
  include($next);
}
?>
