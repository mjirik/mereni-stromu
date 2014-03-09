<?php
// $Id: zarad.tmpl.php 42 2007-06-19 09:29:01Z Mira $


echo "<div class=\"obtekany\">";//tyle=\"padding:10px; float:left;\">";
echo "<div class=\"vnitrni\">";
echo "<h2>$jmeno</h2>";
echo "<h3>$nazev</h3>";
if (!empty($szaznamy)){
  echo "<table cellspacing=0 cellpadding=2>";
  echo "<tr>";
  echo "<th>Délka<br>[m]</th>
      <th>Průměr<br>[cm]</th>
      <th>Druh dřeva</th>
      <th>2x</th>
      <th>Manip</th>";
  if ($confDalsiZpracovani == true){
    echo  "<th>Další<br>zpracování</th>";
  }
  echo "</tr>";
  foreach ($szaznamy as $zaznam){
    echo "<tr";
    echo $zaznam["IDMereni"]==$IDMereni?" class=\"focus\">":">";

    echo "<td align=\"center\">".$zaznam["Delka"]."</td>";
    echo "<td align=\"center\">".$zaznam["Prumer"]."</td>";
    echo "<td align=\"center\">".$zaznam["Druh_dreva"]."</td>";
    
    echo "<td align=center><input type = \"checkbox\" name = \"dvakrat".$zaznam["IDMereni"]."\" value=1";
    if ($zaznam["IDDvakrat"]==1){
      echo " checked";
    }
    echo " disabled></td>";
    
    echo "<td align=center><input type = \"checkbox\" name = \"manipulace".$zaznam["IDMereni"]."\" value=1";
    if ($zaznam["IDManipulace"]==1){
      echo " checked";
    }
    echo " disabled></td>";
    
    if ($confDalsiZpracovani == true){
      echo "<td align=\"center\">".$zaznam["Dalsi_zpracovani"]."</td>";
    }
    
    echo "</tr>";
  }
echo "</table>";
}
echo "</div></div>";
echo "<div class = \"druheOkno\">";
echo "<div class=\"vnitrni\">";//style=\"float:left; padding:10px;\">";
//echo "sldfllkj";
if (!empty($dzakaznici)){
  echo "<table>";
  echo "<tr>
  <th>Jméno</th>
  </tr>";
  foreach($dzakaznici as $zaznam){
    echo "<tr>";
    echo "<td>".$zaznam["Jmeno"]."</td>";
    echo "<td>".odkaz("Zvolit","zarad.php?$src&amp;IDZakaznik=".$zaznam["IDZakaznik"])."</td>";
    echo "</tr>";
  }
  echo "</table>";
}
echo "</div>";
echo "</div>";
if (!empty($dseznamy)){
echo "<div class=\"druheOkno\">";
echo "<div class=\"vnitrni\">";//style=\"float:left; padding:10px;\">";
  echo "<table>";
  echo "<tr>
  <th>Zakázky</th>
  </tr>";
  foreach($dseznamy as $zaznam){
    echo "<tr>";
    echo "<td>".$zaznam["IDSeznam"]." : ".sql_datum($zaznam["Datum"])."</td>";
    echo "<td>".odkaz("Zvolit","zarad.php?$src&amp;IDSeznam=".$zaznam["IDSeznam"])."</td>";
    echo "</tr>";
  }
  echo "</table>";

echo "</div>";
echo "</div>";
}
?>
