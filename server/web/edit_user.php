<?php
// $Id: edit_user.php 100 2007-08-20 18:27:25Z Mira $
/**
 * @file edit_user.php
 * 
 * Soubor zajišťuje editaci uživatele.  
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
if ($_SESSION["id"] == 1){
  $body->set('nonchange', true);
  if (!empty($_GET["iduziv"])){
    $IDUzivatel = $_GET["iduziv"];
  }
  else{
    $IDUzivatel = $_SESSION["id"];
  }
}
else {
  $IDUzivatel = $_SESSION["id"];
}


$nick = user_name($IDUzivatel,$link);

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
  $nick = $_POST["nick"];
}
if ($BudemeZobrazovat){
  
  $body->set('nick', $nick);
  
  $body->set('action', "edit_user.php?iduziv=".$_GET["iduziv"]);
  $body->set('echo', $echo);

  $all->set('nadpis', "Editace účtu");
  $all->set('body', $body);

  echo $all->fetch();
}
else{
  edit_user($IDUzivatel,$_POST["nick"],$_POST["heslo1"],$link);
  include("users.php");
}

?>
