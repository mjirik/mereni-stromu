<?php
// $Id: users.php 100 2007-08-20 18:27:25Z Mira $
/**
 * @file user.php
 * Soubor zajišťuje vypisování seznamu uživatelů. 
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}

if($_SESSION["id"]==1){
  include_once ('./class/template.class.php');
  $all = & new Template('./tmpl/all.tmpl.php');
  $body = & new Template('./tmpl/users.tmpl.php');
  //include_once ("connect_info.php");
  include_once ('./inc/funkce.inc.php');
  $link = connect_db();
  
  if (!empty($_GET["deliduser"]) && ($_GET["deliduser"] > 1)){
    delete_user($_GET["deliduser"],$link);
  }
  
  $users = users($link);
  
  $body->set('users', $users);
  $all->set('body', $body);

  echo $all->fetch();

}
?>
