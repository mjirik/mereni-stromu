<?php
// $Id: add_zakaznik.php 73 2007-08-12 14:35:42Z Mira $
/**
 * @file add_zakaznik.php
 * 
 * Skript zajišťuje přidávání zákazníků.
 */
include ('./inc/funkce.inc.php');
$BudemeZobrazovat=true;
if (!empty($_POST)): //už se odesílalo a musíme kontrolovat
  //kontroly
  //include ("./inc/connect_info.inc.php");
  $link = connect_db();

  $check = insertZakaznik($_POST,$link);
  if (is_numeric($check)){
    $BudemeZobrazovat = false;
    
    include ("zakazky.php");
  }
endif;

if ($BudemeZobrazovat):
include ('./class/template.class.php');
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/edit_zakaznik.tmpl.php');

$body->set('zakaznik', $_POST);
$body->set('echo', $check);
$body->set('action', "add_zakaznik.php");
$body->set('samostatne', true);

$all->set('body', $body);
echo $all->fetch();

//Prošli jsme kontrolami
else:
//echo "Zpracovávám ".$_POST["jmeno"];  



// mysql_query("insert into Zakaznik (Jmeno,ICO,DIC,Adresa,Telefon,Email) values (\"".
// $_POST["jmeno"]."\",\"".
// $_POST["ico"]."\",\"".
// $_POST["dic"]."\",\"".
// $_POST["adresa"]."\",\"".
// $_POST["telefon"]."\",\"".
// $_POST["email"]."\");")
// or die("Nelze vykonat definicní dotaz: " . mysql_error());
// 
// if ($_SESSION["next"] != null){
//   include ($_SESSION["next"]);
// }
// else{
//   
// }

endif
?>

