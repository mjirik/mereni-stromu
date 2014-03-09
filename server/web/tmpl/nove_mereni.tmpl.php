<h3>Nový řádek</h3>
  <table>
  <tr><th>Délka<br>[m]</th><th>Průměr<br>[cm]</th><th>Druh dřeva</th><th>2x</th><th>Manip</th>
  <?php if ($confDalsiZpracovani==true){ ?>
  <th>Další zpracování</th>
  <?php } ?>
  </tr>
  <tr>
<?php
  echo "<td align=center>"."<input name=\"delka\" size = 2 value=\"".$zaznam["Delka"]."\">"."</td>";
  echo "<td align=center>"."<input name=\"prumer\" size = 2 value=\"".$zaznam["Prumer"]."\">"."</td>";

  echo "<td><select name=\"druh\">";
  foreach ($zdruhy as $druh){
    echo "<option value =\"".$druh["IDDruh_dreva"]."\"";
    if ($druh["IDDruh_dreva"] == $zaznam["IDDruh_dreva"]){
      echo " selected";
    }
    echo ">".$druh["Nazev"]."</option>";
  }
  echo "</select></td>";
  echo "<td align=center><input type = \"checkbox\" name = \"dvakrat\" value=1";
  if ($zaznam["IDDvakrat"]==1){
    echo " checked";
  }
  echo "></td>";
  
  echo "<td align=center><input type = \"checkbox\" name = \"manipulace\" value=1";
  if ($zaznam["IDManipulace"]==1){
    echo " checked";
  }
  echo "></td>";

  
  // dalsi zpracovani
  if ($confDalsiZpracovani==true){
  echo "<td><select name=\"zpracovani\">";
//  echo "<option value =\"null\"";
//  if (null == $zaznam["IDDalsi_zpracovani"]){
//    echo " selected";
//  }
  foreach ($dalsi_zpracovani as $zpracovani){
    echo "<option value =\"".$zpracovani["IDDalsi_zpracovani"]."\"";
    if ($zpracovani["IDDalsi_zpracovani"] == $zaznam["IDDalsi_zpracovani"]){
      echo " selected";
    }
    echo ">".$zpracovani["Nazev"]."</option>";
  }
  echo "</select></td>";
  }
  
  if ($zaznam["Chyba"] == true){
    echo "<td><font color = \"red\">chyba</td>";
  }

  echo "</tr>\n";
  echo "</table>";
  //end_form();
  //echo "</form>";
  submit("Vložit","novy");
?>
