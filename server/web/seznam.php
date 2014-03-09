<?php
// $Id: seznam.php 112 2007-08-29 08:20:39Z Mira $
/**
 * @file seznam.php
 * Soubor vypisuje náhlled tisku. 
 * 
 *   
 */
session_start();
if (!session_is_registered("id")){
  include ("login.php");
  die();
}


include ('./class/template.class.php');
$all = & new Template('./tmpl/print.tmpl.php');
$body = & new Template('./tmpl/seznam.tmpl.php');

include ('./inc/funkce.inc.php');

//include ("connect_info.php");
$link = connect_db();

$seznam = seznam_mereni($_GET["id"],$link,true);
$param = parametry_seznamu($_GET["id"],$link);
$zakaznik = zakaznik($param["IDZakaznik"],$link);
$parametry = druhy_dreva($link);
$cenaCelkem = 0;
$mezisoucty = NULL;
//var_dump($parametry);
if (!empty($seznam)){
  foreach ($seznam as $key => $zaznam){
    $p0 = $parametry[$zaznam["IDDruh_dreva"]-1]["P0"];
    $p1 = $parametry[$zaznam["IDDruh_dreva"]-1]["P1"];
    $p2 = $parametry[$zaznam["IDDruh_dreva"]-1]["P2"];
    $zaznam["Objem"] = objem ($zaznam["Delka"],$zaznam["Prumer"],$zaznam["P0"],$zaznam["P1"],$zaznam["P2"]);
    $zaznam["Cena"] = cena_strom($zaznam,$param);
    $seznam[$key]=$zaznam;
    
    /// @todo Zde by se nechalo optimalizovat - některé položky se vypočítávají 
    /// pro každé měření znovu. Stačilo by počítat jednou.

    //Zde jsou řečeny mezisoučty pro jednotlivé skupiny
    $mezisoucty[$zaznam["IDDruh_dreva"]>3][$zaznam["IDManipulace"]]
    [$zaznam["IDDvakrat"]][$zaznam["IDDalsi_zpracovani"]]["Cena"] += $zaznam["Cena"];
    //objem
    $mezisoucty[$zaznam["IDDruh_dreva"]>3][$zaznam["IDManipulace"]]
    [$zaznam["IDDvakrat"]][$zaznam["IDDalsi_zpracovani"]]["Objem"] += $zaznam["Objem"];
    //Cena/m3
//    $mezisoucty[$zaznam["IDDruh_dreva"]][$zaznam["IDManipulace"]]
//    [$zaznam["IDDvakrat"]][$zaznam["IDDalsi_zpracovani"]]["Popisek"]= cena_strom_m3($zaznam,$param);

    //popisek 
    $mezisoucty[$zaznam["IDDruh_dreva"]>3][$zaznam["IDManipulace"]]
    [$zaznam["IDDvakrat"]][$zaznam["IDDalsi_zpracovani"]]["Popisek"]=
    "<i>".(($zaznam["IDDruh_dreva"] > 3)?"Tvrdé dřevo":"Měkké dřevo").(($zaznam["IDManipulace"]==1)?", man":"").
        (($zaznam["IDDvakrat"]==1)?", 2x":"").($zaznam["IDDalsi_zpracovani"]>1?", ":"").
        ($zaznam["Dalsi_zpracovani"])." (".cena_strom_m3($zaznam,$param)." kč/m³) </i>";
     //(($zaznam["IDDvakrat"]==1)?", 2x":"")
    $cenaCelkem = $cenaCelkem + $zaznam["Cena"];
  }
}
  $body->set('zakaznik', $zakaznik);
  $body->set('zaznamy', $seznam);
  $body->set('cenaCelkem', $cenaCelkem);
  $body->set('mezisoucty', $mezisoucty);
  
  $body->set('confPodrobnyTisk', $confPodrobnyTisk);

  $all->set('body', $body);
  echo $all->fetch();


?>
