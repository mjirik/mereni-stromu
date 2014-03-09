<?php
// $Id: ceny.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file ceny.php 
 * Soubor zajišťuje editaci cen.  
 */ 
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}

include_once ('./class/template.class.php');
include_once ('./inc/funkce.inc.php');
$link = connect_db();
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/ceny.tmpl.php');
$zdruhy = druhy_dreva($link);
$ceny = getCeny($link);

if (!empty($_POST)){//testy
  $ok = true;
  // testování správného zadání cen
  $poleRetezcu[]="Cena_1";
  //$poleRetezcu[]="Cena_2";
  //$poleRetezcu[]="Cena_3";
  $poleRetezcu[]="Cena_4";
  //$poleRetezcu[]="Cena_5";
  $poleRetezcu[]="Cena_man";
  if ($confDalsiZpracovani != false){
    $poleRetezcu[]="Cena_dalsi_1";
    $poleRetezcu[]="Cena_dalsi_2";
  }
  $poleRetezcu[]="Cena_dvakrat";
  
  foreach($poleRetezcu as $retezec){
    $cena = StrTr(trim($_POST[$retezec]),",",".");
    if (jeCislo($cena)&&($cena < 10000)){
      $ceny[$retezec]= $cena;
    }
    else{
      //echo $retezec."ahoj".$cena;
      $echo[] = "Cena musí být číslo menší než 10000";
      $ok = false;
    }
  }
  
  if ($ok){
    setCeny($ceny,$link);
  }
}

$body->set('zdruhy', $zdruhy);
$body->set('ceny', $ceny);
$body->set('echo', $echo);
$body->set('confDalsiZpracovani', $confDalsiZpracovani);
  
$all->set('body', $body);
$all->set('nadpis', "Ceník");

echo $all->fetch();
?>
