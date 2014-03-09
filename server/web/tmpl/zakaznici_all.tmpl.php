<?php
if (!empty($echo)){
  echo $echo;
}


if ($_SESSION["archivZakaznik"]==1){
  echo link("Skryj archiv","zakaznici.php?archivZakaznik=-1")."<br>";
}
else{
  echo odkaz("Zobraz archiv","zakaznici.php?archivZakaznik=1")."<br>";
}

  echo odkaz("Nový zákazník","add_zakaznik.php","Přidání nového zákazníka");
echo "<hr size=3 align=\"center\">"; 
if (!empty($zaznamy)){
  echo "<div class=\"vnitrni\" style=\"float:left\">";//" class=\"vnitrni\">";
  echo "<table>";
  foreach ($zaznamy as $zaznam){
    if ($id == $zaznam["IDZakaznik"]){
      echo "<tr class = \"orange\">";
    }
    else {
      echo "<tr>";
    }
    echo "<td>";
    if (bit2bool($zaznam["Archiv"])){
      echo odkaz($zaznam["Jmeno"],"zakazky.php?id=".$zaznam["IDZakaznik"],"Zobrazí zakázky zákazníka","archiv");
    }
    else{
      echo odkaz($zaznam["Jmeno"],"zakazky.php?id=".$zaznam["IDZakaznik"],"Zobrazí zakázky zákazníka");
    }
    echo "</td><td>";
    echo "<a href = zakaznik.php?id=".$zaznam["IDZakaznik"].">Zobrazit</a>\n";
    echo "</td><td>";
    echo "<a href = edit_zakaznik.php?id=".$zaznam["IDZakaznik"].">Upravit</a>\n";
    echo "</td><td>";
    echo "<a href = zakaznici.php?delid=".$zaznam["IDZakaznik"].">"."Smazat"." </a>\n";
    echo "</td>";
    
//     echo "<br>\n";
    echo "</tr>";
  }
  echo "</table>";
  echo "</div >";  
  echo "<div style=\"float:left; background-color: #fc3\">";
  echo $sez_div;
  echo "</div>";
}
?>
