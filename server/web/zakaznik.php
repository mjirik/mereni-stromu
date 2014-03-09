<?php
// $Id: zakaznik.php 58 2007-08-03 10:29:44Z Mira $
/**
 * @file zakaznik.php
 * Skript implementuje výpis zákazníka.
 * 
 * @todo Zkontrolovat využívání skriptu.  
 */
include_once ('./class/template.class.php');
$all = & new Template('./tmpl/all.tmpl.php');
$body = & new Template('./tmpl/zakaznik.tmpl.php');

include_once ('./inc/funkce.inc.php');
$link = connect_db();
if (is_numeric($_GET["id"])){
  $zaznam=zakaznik($_GET["id"],$link);
  if ($zaznam == null){
    $echo = "Nenalezen odpovídající řádek!";
  }
 
  $body->set('echo', $echo);
  $body->set('zaznam', $zaznam);

  $all->set('body', $body);

  echo $all->fetch();

}
else{
  // vyskytne se pokud se někdo pokusí do adresy připsat něco pitomýho
  echo "Chyba při předávání identifikátoru!";
}

?>
