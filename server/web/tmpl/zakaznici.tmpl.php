<?php
if (!empty($echo)){
  echo $echo;
} 
  echo "<div class=\"\" style=\"float:left\">";//" class=\"vnitrni\">";
  echo "<table>";
  foreach ($zaznamy as $zaznam){
    if ($id == $zaznam["IDZakaznik"]){
      echo "<tr class = \"focus\">";
    }
    else {
      echo "<tr>";
    }
    echo "<td>";
    echo "<a href = ".$href.$zaznam["IDZakaznik"].">".$zaznam["Jmeno"]." </a>\n";
    echo "</td>";
//     echo "<br>\n";
    echo "</tr>";
  }
  echo "</table>";
  echo "</div >";
?>
