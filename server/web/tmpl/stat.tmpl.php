<?php // $Id: stat.tmpl.php 49 2007-08-02 08:26:45Z Mira $ 
?>
<div class="vnitrni" style="float:left">
  <table>
  <tr>
  <td><a>ahoj</a></td>
  <td><a>ahoj</a></td>
  </tr>
  </table>
  <?php   
  $kalendar = new kalendar();
  echo $kalendar->vypis(7,2007); 
  ?>
</div>
<div class="druheOkno">
  <div class="vnitrni" style="float:left;">
    <table>
    <tr>
    <th>Jméno</th><th>Zkrácené</th><th>Položek</th><th>Zkratek</th>
    </tr>
<?php    
    $datum = null;
  
    foreach($zaznamy as $zaznam){
      if (strcmp($datum,$zaznam["Datum"]) != 0){
        echo "<tr><td><h4>".sql_datum($zaznam["Datum"])."</h4></td></tr>";
      }
      $datum = $zaznam["Datum"]; 
?>
      <tr>
      <td><?php echo $zaznam["Jmeno"]; ?></td>
      <td><?php echo $zaznam["Zkratka"]; ?></td>
      <td><?php echo $zaznam["Pocet_zkratek"]; ?></td>
      <td><?php echo $zaznam["Pocet_stromu"];?></td>
      </tr>
<?php
    }
?>

    </table>

  </div>
</div>
