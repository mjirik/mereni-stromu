<?php
if (!empty($zzaznamy)){
  $hr = "";
  if (!empty($_GET["id"])){
    $hr = "&amp;id=".$_GET["id"];
  }
  if (!empty($zzaznamy["echo"])){
    echo "<div class=\"chyba_vnitrni\">";
    echo $zzaznamy["echo"];
    echo "</div>";
  }
  else{
    if (empty($href)){
      $href = "seznam.php?id=";
    }
    echo "<table class=\"longtab\">"; 
    foreach($zzaznamy as $zaznam){
      if (bit2bool($zaznam["Archiv"])){
        echo "<tr class=\"archiv\">";
      }
      else{
        echo "<tr>";
      }
      echo "<td align=right>";
      echo $zaznam["IDSeznam"]." : ".sql_datum($zaznam["Datum"]);
      echo "</td>";
      echo "<td>";
      //var_dump($zaznam);
      //echo bin2hex($zaznam["Archiv"]);
      echo odkaz("Zobrazit",$href.$zaznam["IDSeznam"]);

      
      // tlačítka

      echo "</td><td>";
      echo odkaz("Přesunout","zarad.php?sIDSeznam=".$zaznam["IDSeznam"],
      "Přesune seznam k jinému zákazníkovi");
      
      echo "</td><td>";
      $ok = "zakazky.php?delSezID=".$zaznam["IDSeznam"]."$hr";
      $storno = "zakazky.php?id=".$_GET["id"];
      echo ref_ok("Smazat",$ok,$storno);
      //echo odkaz("Smazat","zakazky.php?delSezID=".$zaznam["IDSeznam"]."$hr");
            echo "</td><td>";
      if (!bit2bool($zaznam["Archiv"])){
        echo odkaz("Do archivu","zakazky.php?archSezID=".$zaznam["IDSeznam"]."$hr",
          "Přesune zákázku do archivu");
      }
      else {
        echo odkaz("Z archivu","zakazky.php?archSezID=".-$zaznam["IDSeznam"]."$hr",
        "Vrátí zakázku z archivu");

      }
      echo "</td></tr>";
    }
    echo "</table>";
  }
}

?>
