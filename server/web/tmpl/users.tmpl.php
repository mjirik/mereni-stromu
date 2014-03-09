<?php
// $Id: users.tmpl.php 100 2007-08-20 18:27:25Z Mira $

echo "<a href = add_user.php> Přidej uživatele </a><br><br>";
if (!empty($users)){
  echo "<table>";
  foreach ($users as $user){
    echo "<tr><td>"
    .$user["Nick"]."</td><td>"
    .odkaz("Edit","edit_user.php?iduziv=".$user["IDUzivatel"])."</td>";
    if ($user["IDUzivatel"] > 1){
      echo "<td>".odkaz("Delete","users.php?deliduser=".$user["IDUzivatel"])."</td>";
    }
    echo "</tr>";
  }
  echo "</table>";
}
?>
