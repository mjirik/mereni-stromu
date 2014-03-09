<?php
// $Id: ceny.tmpl.php 100 2007-08-20 18:27:25Z Mira $
?>
<?php if (!empty($echo )){ ?>
  <div class ="chyba">
  <?php echo_echo($echo); ?>
  </div>
<?php } ?> 
<div class="vnitrni">
<form method="post" action="ceny.php">
<h3>Ceny [kč/m³]</h3>
<table>
<?php
formln("Tvrdé",
"<input name=\"Cena_1\" size = 4 value=\"".$ceny["Cena_1"]."\">");
//formln($zdruhy[1]["Nazev"]."",
//"<input name=\"Cena_2\" size = 4 value=\"".$ceny["Cena_2"]."\">");
//formln($zdruhy[2]["Nazev"]."",
//"<input name=\"Cena_3\" size = 4 value=\"".$ceny["Cena_3"]."\">");
formln("Měkké",
"<input name=\"Cena_4\" size = 4 value=\"".$ceny["Cena_4"]."\">");
//formln($zdruhy[4]["Nazev"]."",
//"<input name=\"Cena_5\" size = 4 value=\"".$ceny["Cena_5"]."\">");
formln("Manipuace",
"<input name=\"Cena_man\" size = 4 value=\"".$ceny["Cena_man"]."\">");
if ($confDalsiZpracovani == true){
formln("Sámovka",
"<input name=\"Cena_dalsi_1\" size = 4 value=\"".$ceny["Cena_dalsi_1"]."\">");
formln("Pásovka",
"<input name=\"Cena_dalsi_2\" size = 4 value=\"".$ceny["Cena_dalsi_2"]."\">");
}
formln("Dvakrát",
"<input name=\"Cena_dvakrat\" size = 4 value=\"".$ceny["Cena_dvakrat"]."\">");
submit_form("Uložit");
?>
</table>
</form>
</div>
