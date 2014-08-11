<?php
echo "login ok";
?>
<?php
// $Id: login.php 100 2007-08-20 18:27:25Z Mira $
/**
 * @file login.php
 * Vykreslení přihlašovacího okna a zajištění regulérního přihlášení. 
 */
include_once ("./inc/funkce.inc.php");
$Link = connect_db();
if (empty($Link)){
  $error = "Selhalo spojení s databází";
}

$BudemeZobrazovat = true;

if (!empty($_POST) and (!empty($Link))){
  $ID =iduzivatele($_POST["nick"], $_POST["heslo"], $Link);
  
  /// @todo odstranit roota jako uživatele 0 
  //if (isroot($_POST["nick"], $_POST["heslo"]) == true){
  //  $ID = 0;
  //}
  
  if ($ID == -1){
    echo "Uživatelské jméno anebo heslo nesouhlasí<br>" ;
  }
  else{
    $BudemeZobrazovat=false;
    session_start();
    session_register("id");
    session_register("link");
    session_register("page");
    session_register("archivSeznam");
    session_register("archivZakaznik");
    session_register("ok_text");
    session_register("ok_hrefOk");
    session_register("ok_hrefStorno");
    $_SESSION["id"]= $ID;
    $_SESSION["link"]= $Link;
    $_SESSION["archivSeznam"]= 0;
    $_SESSION["archivZakaznik"]= 0;
    include("zakazky.php");
  }
}


if ($BudemeZobrazovat){ ?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title> index </title>
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="login">
<?php
  if (!empty($error)){
    echo "<div class=\"chyba\">";
    echo "Error: ";
    echo $error;
    echo "</div>"; 
  }
  else {
    echo "<div class=\"obtekany\">";
    echo "<div class=\"vnitrni\">";
    echo "<form method=\"post\" action=\"login.php?\">";
    echo "<table>";
    formln("Jméno:","<input name=\"nick\" >");
    formln("Heslo:","<input type=password name=\"heslo\" >");
    formln("","<input type=\"Submit\" name=\"odesli\" size = 10 value = \"Login\">");
    echo "</table>";
    echo "</form>";
    //echo $_SERVER["REMOTE_ADDR"];
    echo "</div>";
    echo "</div>";
  }
?>
</div>
</body>
</html>
<?php } ?>
<?php
echo "<hmtl>" ;
echo "<body>";
echo"test";
echo"</body>";
echo "</html>";
?>
