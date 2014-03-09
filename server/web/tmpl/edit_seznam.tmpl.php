<?php
// $Id: edit_seznam.tmpl.php 100 2007-08-20 18:27:25Z Mira $

echo "<form method=\"post\" action=\"edit_seznam.php?id=".$id;
if ($_GET["zobraz_novy"] == 1){
  echo "&amp;zobraz_novy=1";
}
echo "\">";
?>
<div>
<div class="obtekany">
<?php
if ($chybovaHlaskaCena != false){
  echo "<div class=\"chyba\">";
  foreach($chybovaHlaskaCena as $hlaska){
    echo $hlaska."<br>";
  }
  echo "</div>";
}
if ($chybovaHlaska != false){
  echo "<div class=\"chyba\">";
  foreach($chybovaHlaska as $hlaska){
    echo $hlaska."<br>";
  }
  echo "</div>";
}
?>
<div class="vnitrni">
<?php //echo "<h2>".$ceny["Jmeno"]."</h2>\n";?>

<table cellspacing=0 cellpadding=0>
<tr><td>
<?php echo odkaz("Seznamy zakázek","zakazky.php?id=".$ceny["IDZakaznik"],
"Vrátí se na seznamy zakázek tohto zákazníka","tlacitko"); ?>
</td></tr><tr><td>
<?php echo odkaz("Tisk","seznam.php?id=$id","Zobrazí tiskový náhled","tlacitko")."\n";
echo "</td></tr><tr><td>\n";
if ($_GET["zobraz_novy"]==1){
  echo odkaz("Skrýt nový řádek","edit_seznam.php?id=$id&amp;zobraz_novy=0",
  "Skryje formulář pro přidávání nového řádku","tlacitko");
}
else{
  echo odkaz("Vložit nový řádek","edit_seznam.php?id=$id&amp;zobraz_novy=1",
  "Zobrazí formulář pro přidávání nového řádku","tlacitko");
}?>
</td></tr>

</table>
<h3 title="Ceny platné pro tuto zakázku">Ceny [kč/m³]</h3>
<table>

<?php formln("Měkké",
"<input id=\"Cena_1\" name=\"Cena_1\" size = 4 value=\"".$ceny["Cena_1"]."\">","Borovice, Smrk");
//formln($zdruhy[1]["Nazev"]."",
//"<input name=\"Cena_2\" size = 4 value=\"".$ceny["Cena_2"]."\">");
//formln($zdruhy[2]["Nazev"]."",
//"<input name=\"Cena_3\" size = 4 value=\"".$ceny["Cena_3"]."\">");
formln("Tvrdé",
"<input id=\"Cena_4\" name=\"Cena_4\" size = 4 value=\"".$ceny["Cena_4"]."\">","Dub, Buk");
//formln("Tvrdé",
//"<input name=\"Cena_5\" size = 4 value=\"".$ceny["Cena_5"]."\">");
formln("Manipulace",
"<input id=\"Cena_man\" name=\"Cena_man\" size = 4 value=\"".$ceny["Cena_man"]."\">");
if ($confDalsiZpracovani==true){
  formln("Sámovka",
  "<input id=\"Cena_dalsi_1\" name=\"Cena_dalsi_1\" size = 4 value=\"".$ceny["Cena_dalsi_1"]."\">");
  formln("Pásovka",
  "<input id=\"Cena_dalsi_2\" name=\"Cena_dalsi_2\" size = 4 value=\"".$ceny["Cena_dalsi_2"]."\">");
}
formln("2 x katrem",
"<input id=\"Cena_dvakrat\" name=\"Cena_dvakrat\" size = 4 value=\"".$ceny["Cena_dvakrat"]."\">");

if (empty($zaznamy)&&($_GET["zobraz_novy"] != 1)){
  formln("","<input class=\"tlacitko\" type=\"Submit\" name=\"odesli\" size = 10 value = \"Uložit\">");
}
?>

</table>
</div>

</div> <?php //obtekany ?>
</div>

<div class="druheOkno">
<?php if ($_GET["zobraz_novy"]==1){
  $disabled = " disabled" 
  ?>

  <div><div class="vnitrni" style="padding:10px;">
  <?php // template pro nový řádek
  echo $novy; ?>
  </div></div>
<?php 
}
if (!empty($zaznamy)){ ?>
<div class = "vnitrni">
<?php
//echo "<h3>$id : ".sql_datum($ceny["Datum"])."</h3>";
?>
 
  <table class="tab_edit_seznam" cellspacing=0 cellpadding=0>
  
  <tr>
  <th>Délka<br>[m]</th>
  <th>Průměr<br>[cm]</th>
  <th>Druh dřeva</th>
  <th title="2x katrem">2x</th>
  <th title="Manipulace">Mnp</th>
  <?php
  if ($confDalsiZpracovani==true){
    echo "<th>Další zpracování</th>";
  }?>
  <th>Cena<br>[kč/m³]</th>
  </tr>

  <?php
    foreach ($zaznamy as $zaznam){
    //echo "<tr>";
    if ($zaznam["Chyba"] == true){
      echo "<tr class=\"chyba\">";
      //echo "<td><font color = \"red\">chyba</td>";
    }
    else {
      echo "<tr>";
    }
    $prepoctiCenu = "\"prepoctiCenu('druh".$zaznam["IDMereni"].
      "','dvakrat".$zaznam["IDMereni"]."','manipulace".$zaznam["IDMereni"].
      "','zpracovani".$zaznam["IDMereni"]."','cena".$zaznam["IDMereni"]."');\""; 
    echo "<td align=center>"."<input name=\"delka".$zaznam["IDMereni"]."\" size = 2 value=\"".$zaznam["Delka"]."\"$disabled>"."</td>";
    echo "<td align=center>"."<input name=\"prumer".$zaznam["IDMereni"]."\" size = 2 value=\"".$zaznam["Prumer"]."\"$disabled>"."</td>";
    echo "<td><select id=\"druh".$zaznam["IDMereni"]."\" name=\"druh".$zaznam["IDMereni"]."\" onChange=$prepoctiCenu>";
    foreach ($zdruhy as $druh){
      echo "<option value =\"".$druh["IDDruh_dreva"]."\"";
      if ($druh["IDDruh_dreva"] == $zaznam["IDDruh_dreva"]){
        echo " selected";
      }
      echo "$disabled>".$druh["Nazev"];//."</option>";
    }
    echo "</select></td>";
    echo "<td align=center><input type = \"checkbox\" name = \"dvakrat".$zaznam["IDMereni"]."\" value=1".
    " id = \"dvakrat".$zaznam["IDMereni"]."\" onclick=$prepoctiCenu";
    if ($zaznam["IDDvakrat"]==1){
      echo " checked";
    }
    echo "$disabled></td>";
    
    echo "<td align=center><input type = \"checkbox\" name = \"manipulace".$zaznam["IDMereni"]."\" value=1".
    " id = \"manipulace".$zaznam["IDMereni"]."\" onclick=$prepoctiCenu";
    if ($zaznam["IDManipulace"]==1){
      echo " checked";
    }
    echo "$disabled></td>";
    
    // dalsi zpracovani
    if ($confDalsiZpracovani==true){
      echo "<td><select id=\"zpracovani".$zaznam["IDMereni"].
        "\" name=\"zpracovani".$zaznam["IDMereni"]."\" onChange=$prepoctiCenu>";
      foreach ($dalsi_zpracovani as $zpracovani){
        echo "<option value =\"".$zpracovani["IDDalsi_zpracovani"]."\"";
        if ($zpracovani["IDDalsi_zpracovani"] == $zaznam["IDDalsi_zpracovani"])
        {
          echo " selected";
        }
        echo "$disabled>&nbsp;".$zpracovani["Nazev"]." </option>";
      }
      echo "</select></td>";
    }
    
    
    echo "<td>";
    
    echo "<span id=\"cena".$zaznam["IDMereni"]."\">".$zaznam["Cena_m3"]."</span>";
    echo "</td>";
    echo "<td>";
    //echo odkaz("Smazat","edit_seznam.php?id=$id&amp;delIDMereni=".$zaznam["IDMereni"]);
//     $ok = urlencode("edit_seznam.php?id=$id&amp;delIDMereni=".$zaznam["IDMereni"]."");
//     $storno = urlencode("edit_seznam.php?id=$id");
//     
//     echo odkaz("Smazat","ok.php?ok_hrefOk=$ok&amp;ok_hrefStorno=$storno");
    $ok = "edit_seznam.php?id=$id&amp;delIDMereni=".$zaznam["IDMereni"];
    $storno = "edit_seznam.php?id=$id";
    $_SESSION["ok_text"] = "Opravdu chcete řádek smazat?";
    if ($_GET["zobraz_novy"] != 1){
      echo ref_ok("Smazat",$ok,$storno,null,"Smaže řádek");
      echo "</td><td>";
      echo odkaz("Přesunout","zarad.php?sIDMereni=".$zaznam["IDMereni"]);
      echo "</td>";
    
    }
    echo "</tr>\n";
    
  }
  

echo "</table>";
if ($_GET["zobraz_novy"] != 1){ ?>
  <input class="tlacitko" type="Submit" name="odesli" size = 10 value = "Uložit" accesskey="u" title="Alt + u">
<?php } ?>
  </div>
<?php } ?>
</div>
</form>
