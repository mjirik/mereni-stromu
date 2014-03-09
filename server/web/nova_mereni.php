<?php
// $Id: nova_mereni.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file nova_mereni.php
 * Skript hledá nová měření a vykresluje formuláž pro jejich přiřazení do 
 * systému.  
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}
include_once ('./inc/funkce.inc.php');
include_once ('./class/template.class.php');
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/volni_zakaznici_mereni.tmpl.php');
$add = null;

$link = connect_db();

session_start();
$_SESSION["ok_text"]="";
$_SESSION["ok_hrefOk"]="";
$_SESSION["ok_hrefStorno"]="";//"Opravdu chcete přiřadit zákaníka?";
// include ("connect_info.php");
// $link = mysql_connect(SQL_HOST, SQL_USERNAME, SQL_PASSWORD);
// mysql_select_db(SQL_DBNAME);
// print_r($_GET);

if (!empty($_GET["id"])){
  $jmeno = jmeno_zakaznik_mereni($_GET["id"],$link);
}
if ((!empty($_GET["IDSeznam"])) && (!empty($_GET["id"]))){
  
  if (($_GET["IDSeznam"] == -1)&&(!empty($_GET["IDZakaznik"]))){
    $idseznam = novySeznam($_GET["IDZakaznik"],$link);
  }
  else{
    $idseznam = $_GET["IDSeznam"];
  } 
  zarad_mereni_do_seznamu($idseznam,$_GET["id"],$link);
  $_GET = null;
}


if (!empty($_GET["IDZakaznik"]) && !empty($_GET["id"])){
  $zakazky = seznamy_zakaznika($_GET["IDZakaznik"],$link);
  seznamy_zakaznika($_GET["IDZakaznik"],$link);
  
  //zarad_mereni_do_seznamu($_GET["IDZakaznik"],$_GET["id"],$link);
}

if (!empty($_GET["id"])&& $_GET["novy"]!=1){
  $zakaznici = zakaznici($link,false);
  //$jmeno = jmeno_zakaznik_mereni($_GET["id"],$link);
  //$add = & new Template('./tmpl/zakaznici.tmpl.php');
  $body -> set ('zakaznici',$zakaznici);
  $body -> set ('jmeno',$jmeno);

}

$check = null;
$zobraz = true;
$zobrazit = true;
if (($_GET["add"]==1)&&(is_numeric($_GET["id"]))){
  $check = insertZakaznik($_SESSION["novy_zakaznik"],$link);
  //var_dump($_SESSION["novy_zakaznik"]);
  if (false == session_unregister("novy_zakaznik")){
    echo "nejde odstranit proměnná ze session"; 
  }
  if (is_numeric($check)){
    //ok vlozeno, priradime zaznam
    zarad_mereni_do_seznamu(novySeznam($check,$link),$_GET["id"],$link);
    $_GET = null;
    $zobraz = false;
  }
}

if (!empty($_GET["id"]) && ($_GET["novy"] == 1)){
  if (!empty($_POST)){
    
    $check = check_zakaznik($_POST);
    if (sizeof($check)==0){
      session_register("novy_zakaznik");
      $_SESSION["novy_zakaznik"]=$_POST;
      //var_dump($_SESSION["novy_zakaznik"]);
      $zobrazit = false;
      $body = & new Template('./tmpl/ok.tmpl.php');
      $body -> set ('hrefOk',"nova_mereni.php?id=".$_GET["id"]."&amp;add=1");
      $body -> set ('hrefStorno',"nova_mereni.php");
      //$jmeno = jmeno_zakaznik_mereni($_GET["id"],$link);
      $body -> set ('text',"Opravdu přiřadit měření od identifikátoru 
      <b>$jmeno</b> <br> do zakázky nového zákaníka <b>".$_POST["jmeno"]."</b>?");
      $all->set('body', $body);
      echo $all->fetch();
      
    }

//    $check = insertZakaznik($_POST,$link);
//    if (is_numeric($check)){
//      //ok vlozeno, priradime zaznam
//      zarad_mereni_do_seznamu($check,$_GET["id"],$link);
//      $zobraz = false;
//    }
  }
  if ($zobraz){    
    //$jmeno = jmeno_zakaznik_mereni($_GET["id"],$link);
    //$add = & new Template('./tmpl/volni_zakaznici_mereni_add.tmpl.php');
    $add = & new Template('./tmpl/edit_zakaznik.tmpl.php');
    $add -> set ('action',"nova_mereni.php?novy=1&amp;id=".$_GET["id"]);
    //$add -> set ('id',$_GET["id"]);
    $add -> set ('jmeno',$jmeno);
    $add -> set ('echo',$check);
    $add -> set ('zakaznik',$_POST);
    $body -> set ('add',$add);
    
  }
}


//mysql_insert_id(&mysql)

if ($zobrazit){
  $zaznamy = volnaMereni($link);
  $body -> set ('zaznamy', $zaznamy);
  $body -> set ('zakazky',$zakazky);
  $body -> set ('jmeno',$jmeno);
  //echo "ssssdsdff  : $jmeno";
  $all->set('body', $body);
  $all->set('nadpis', "Nová měření");
  echo $all->fetch();
}
?>
