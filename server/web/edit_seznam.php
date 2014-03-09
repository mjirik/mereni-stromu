<?php
// $Id: edit_seznam.php 112 2007-08-29 08:20:39Z Mira $
/**
 * @file edit_seznam.php
 * Soubor zajišťuje editaci zakázky.
 */ 

session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}

include_once ('./inc/funkce.inc.php');
$link = connect_db();

// $time_start = getmicrotime();
include_once ('./class/template.class.php');

$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/edit_seznam.tmpl.php');
$tisk = & new Template('./tmpl/seznam.tmpl.php');
$novy = & new Template('./tmpl/nove_mereni.tmpl.php');

//$novy = "heh";

session_start();
//echo getmicrotime();
// if (isset($_SESSION["id"])){
  //$_SESSION["ok_hrefOk"]="edit_seznam.php?id=".$_GET["id"]."&amp;delIDMereni=".//todo
  //$_SESSION["ok_text"]="Opravdu chcete řádek smazat?";
  
  $chybovaHlaska= false;
  $chybovaHlaskaCena= false;
  $ceny = parametry_seznamu($_GET["id"],$link);
  
  if (!empty($_GET["delIDMereni"])){
    deleteMereni($_GET["delIDMereni"],$link);
  }
  
  
  // pokud je přidáván nový řádek
  if (!empty($_POST) && !empty($_POST["novy"])){
    $ok=true;
    //if(!empty($_POST["delka"])){ //Testování jednotlivých položek
    $cislo = StrTr(trim($_POST["delka"]),",",".");
    $nove_mer=null;
    if ((jeCislo($cislo))&&($cislo < 100)){
      $nove_mer["Delka"] = $cislo;
    }
    else{
      $chybovaHlaska[] = "V kolonce 'Délka' musí být zadáno číslo menší než 100!";
      $ok= false;
    }
    $cislo = StrTr(trim($_POST["prumer"]),",",".");
    if ((jeCislo($cislo))&&($cislo < 100)){
      $nove_mer["Prumer"] = $cislo;
    }
    else{
      $chybovaHlaska[] = "V kolonce 'Průměr' musí být zadáno číslo menší než 100!";
      $ok= false;
    }
    $nove_mer["IDDruh_dreva"]=$_POST["druh"];
    if ($_POST["manipulace"] == 1){
      $nove_mer["IDManipulace"] = "1";
    }
    else{
      $nove_mer["IDManipulace"] = "null";
    } 
    if ($_POST["zpracovani"] == null){
      $nove_mer["IDDalsi_zpracovani"] = 1;
    }
    else{
      $nove_mer["IDDalsi_zpracovani"]=$_POST["zpracovani"];
    }
    
    if ($_POST["dvakrat"] == 1){
      $nove_mer["IDDvakrat"] = "1";
    }
    else{
      $nove_mer["IDDvakrat"] = "null";
    } 
    
    if ($ok == true) {
    	new_mereni_do_seznamu($_GET["id"],$nove_mer,$link);
    }
    else {
      $novy->set('zaznam',$nove_mer);
    }
     
  }
  
  $zaznamy = seznam_mereni($_GET["id"],$link,true);

  if ($zaznamy == false){
    $chybovaHlaska[] = "Seznam je prázdný.";
  }
  // $vykreslujeme = true;
  if (!empty($_POST) && empty($_POST["novy"])){//testy
    $vykreslujeme = false;
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
        $vykreslujeme = true;
      }
    }
    if ($vykreslujeme){
       $chybovaHlaskaCena[]="Cena musí být číslo menší než 10000";
    }
    
    //testování správného zadání délky a šířky
    foreach($zaznamy as $key => $zaznam){
      $zaznamy[$key]["Chyba"] = false;
      if(!empty($_POST["delka".$zaznam["IDMereni"]])){ //Testování jednotlivých položek
        $cislo = StrTr(trim($_POST["delka".$zaznam["IDMereni"]]),",",".");
        if ((jeCislo($cislo))&&($cislo < 100)){
          $zaznamy[$key]["Delka"] = $cislo;
        }
        else{
          $zaznamy[$key]["Chyba"] = true;
          $chybovaHlaska[] = "V kolonce 'Délka' musí být zadáno číslo menší než 100!";
          $vykreslujeme = true;
        }
        
        $cislo = StrTr(trim($_POST["prumer".$zaznam["IDMereni"]]),",",".");
        if (jeCislo($cislo)){
          $zaznamy[$key]["Prumer"] = $cislo;
        }
        else{
          $zaznamy[$key]["Chyba"] = true;
          $vykreslujeme = true;
          $chybovaHlaska[] = "V kolonce 'Průměr' musí být zadáno číslo menší než 100!";
        }
        $zaznamy[$key]["IDDruh_dreva"] = $_POST["druh".$zaznam["IDMereni"]];
        if ($_POST["manipulace".$zaznam["IDMereni"]] == 1){
          $zaznamy[$key]["IDManipulace"] = 1;
        }
        else{
          $zaznamy[$key]["IDManipulace"] = false;
        }
//dvakrat
        if ($_POST["dvakrat".$zaznam["IDMereni"]] == 1){
          $zaznamy[$key]["IDDvakrat"] = 1;
        }
        else{
          $zaznamy[$key]["IDDvakrat"] = false;
        } 

         
        $zaznamy[$key]["IDDalsi_zpracovani"] = $_POST["zpracovani".$zaznam["IDMereni"]];
      }
      else{ 
        "Chyba při předávání dat!";
      }
    }
    if ($vykreslujeme == false){
      update_ceny($_GET["id"],$ceny,$link);
      update_mereni($zaznamy,$link);    
    }
  }
  if (!empty($zaznamy)){
    foreach ($zaznamy as $key => $zaznam){
      $zaznam["Objem"] = objem ($zaznam["Delka"],$zaznam["Prumer"],1);
      $zaznam["Cena_m3"] = cena_strom_m3 ($zaznam,$ceny);
      $zaznamy[$key]=$zaznam;
      
    }
  }


  $zdruhy = druhy_dreva($link);
  $dalsi_zpracovani = dalsi_zpracovani($link);

  $body->set('ceny', $ceny);
  $body->set('chybovaHlaskaCena', $chybovaHlaskaCena);
  $body->set('zaznamy', $zaznamy);
  $body->set('zdruhy', $zdruhy);
  $body->set('dalsi_zpracovani', $dalsi_zpracovani);
  $body->set('chybovaHlaska', $chybovaHlaska);
  $body->set('id', $_GET["id"]);
  $body->set('post',$_POST);
  $body->set('confDalsiZpracovani', $confDalsiZpracovani);
  
  $novy->set('zdruhy',$zdruhy);
  $novy->set('zaznam',$nove_mereni);
  $novy->set('dalsi_zpracovani', $dalsi_zpracovani);
  $novy->set('confDalsiZpracovani', $confDalsiZpracovani);
  
  
 
  //$body->set('tisk',$tisk);
  $body->set('novy',$novy);
  $all->set('nadpis',$ceny["Jmeno"]);
  $all->set('nadpis2',$_GET["id"]." : ".sql_datum($ceny["Datum"]));
  $all->set('body', $body);

  
  echo $all->fetch();
?>
