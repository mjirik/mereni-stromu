<?php
if (!empty ($zaznamy)){
  echo "<div style=\"float:left\">";
  $kalendar = new kalendar();
  echo $kalendar->vypis(7,2007);
      echo "<table>";
      echo "<tr>";
      echo "<th>Jméno</th><th>Zkrácené</th><th>Položek</th><th>Zkratek</th>";
      echo "</tr>";
      $datum = null;
  //      ($zaznam=MySQL_Fetch_Array($vysledek)){
      foreach($zaznamy as $zaznam){
        if (strcmp($datum,$zaznam["Datum"]) != 0){
          echo "<tr><td><h4>".sql_datum($zaznam["Datum"])."</h4></td></tr>";
        }
        $datum = $zaznam["Datum"];
        echo "<tr>";
        echo "<td>".$zaznam["Jmeno"]."</td><td>";
        echo $zaznam["Zkratka"]."</td><td>";
        echo $zaznam["Pocet_zkratek"]."</td><td>".
        $zaznam["Pocet_stromu"].
        "</td><td><a href=\"nova_mereni.php?vyber=1&id="
        .$zaznam["IDZakaznik_mereni"]."\">Vyber"."</a></td>";
        echo "<td><a href=\"nova_mereni.php?novy=1&id=".
        $zaznam["IDZakaznik_mereni"].
        "\">Nový zákazník</a></td>";
        if ($zaznam["nej_zakaznici"]!=null){
          foreach($zaznam["nej_zakaznici"] as $nej_zakaznik){
            echo "<td>".ref_ok($nej_zakaznik["Jmeno"],
              "nova_mereni.php?id="
              .$zaznam["IDZakaznik_mereni"]."&amp;IDZakaznik=".
              $nej_zakaznik["IDZakaznik"],
              "nova_mereni.php",
              "Opravdu chcete nahradit zákazníka <b>".$zaznam["Jmeno"]."</b> 
              zákazníkem <b>".$nej_zakaznik["Jmeno"]."</b>?"
              )."</td>";
          
//             echo "<td><a href=\"nova_mereni.php?id=".
//             $zaznam["IDZakaznik_mereni"]."&amp;IDZakaznik=".
//             $nej_zakaznik["IDZakaznik"]."\">".
//             $nej_zakaznik["Jmeno"]."</a></td>";
          }
        }
        echo "</tr>\n";
      }
      echo "</table>";
  echo "</div>";
  echo "<div style=\"float:left\">";
  if (!empty($zakaznici)){
    echo "<table>";
    foreach ($zakaznici as $zaznam){
      echo "<tr>";
      echo "<td>";
//       if ($id == $zaznam["IDZakaznik"]){
//         echo "<tr class = \"focus\">";
//       }
//       else {
//         echo "<tr>";
//       }
//       echo "<td>";
      echo ref_ok($zaznam["Jmeno"],"nova_mereni.php?id=".
      $_GET["id"]."&IDZakaznik=".$zaznam["IDZakaznik"],
      "nova_mereni.php",
      "Opravdu nahradit zákazníka <b>".$jmeno."</b> 
      zákazníkem <b>".$zaznam["Jmeno"]."</b>");
      //echo "<a href = ".$href.$zaznam["IDZakaznik"].">".$zaznam["Jmeno"]." </a>\n";
      echo "</td>";
      echo "</tr>";
    }
    echo "</table>";

  }
  echo $add;
  echo "</div>";
}
else {
  echo "Nenalezen žádný záznam.";
}
?>
