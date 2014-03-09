<?php
// $Id: all.tmpl.php 100 2007-08-20 18:27:25Z Mira $

//zajistí nestartování session při vkládání do jiné stránky
      
if (!session_is_registered("id")){
session_start(); 
}
if (!isset($_SESSION["id"])):
  include ("login.php");
  
else:
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
  <html>
    <head>
    <script src="js/dom.js" type="text/javascript"></script>
    <script src="js/skript.js" type="text/javascript"></script>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title> Dřevo </title>
    <link rel="stylesheet" type="text/css" href="style.css">
    </head>
    <body>
    <div id="head">
    <div id="headLeft">
    
    </div>
    <div id="headMain">
    <h1><?php echo $nadpis;
      if (!empty($nadpis2)){
        echo "<font size=\"4\"> - $nadpis2</font>";
      }?>
    </h1>
   
    </div>
    </div>
   
   
    <div id="body"> 
    <div id="bodyLeft">
    <div id="bodyLeftIn">
    <?php 
    include ("./inc/menu.inc.php"); 
    if ($_SESSION["id"] == 1){
      include ("./inc/menu_root.inc.php");
    }
    ?>
    </div>
    </div>

    <div>
    <div id="bodyMain">
      <?php echo $body; ?>
    </div>
    </div>
    </div> 
   
    <?php endif; ?>
  </body>
</html>
