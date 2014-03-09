<?php
// $Id: add_user.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file add_user.php
 * 
 * $file$
 * $File$  
 * Soubor zajišťuje přidání nového uživatele.    
 */ 

session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}

include ('./class/template.class.php');
include ('./inc/funkce.inc.php');
$link = connect_db();

$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/user.tmpl.php');
$BudemeZobrazovat = true;
$echo = null;
if (!empty($_POST)){
  $BudemeZobrazovat = false;
  if ((strlen(trim($_POST["nick"])) < 3)){
    $echo[] = "Nick musí být alespoň tři znaky dlouhý!";
    $BudemeZobrazovat=true;
  }
  
  // ověření hesla
  if (strcmp($_POST["heslo1"],$_POST["heslo2"])== 0){
    if ((strlen(trim($_POST["heslo1"])) < 3)){
      $echo[] = "Heslo musí být alespoň tři znaky dlouhé!";
      $BudemeZobrazovat=true;
    }
  }
  else{
    $echo[] = "Chyba při potvrzování hesla!";
    $BudemeZobrazovat=true;
    $_POST["heslo1"] = null;
    $_POST["heslo2"] = null;
  }  
}
if ($BudemeZobrazovat){
  $body->set('nick', $_POST["nick"]);
  $body->set('action', "add_user.php");
  $body->set('echo', $echo);

  $all->set('body', $body);

  echo $all->fetch();
}
else{
  add_user($_POST["nick"],$_POST["heslo1"],$link);
  include("users.php");
}

?>
