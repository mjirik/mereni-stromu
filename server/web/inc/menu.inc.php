<?php
// $Id: menu.inc.php 100 2007-08-20 18:27:25Z Mira $
/** @file menu.inc.php
 *
 *  Soubor je templatem pro menu.
 */
?>
<ul>
<li><a class="menu" href = nova_mereni.php> Měření </a></li>
<li><a class="menu" href = zakazky.php title="Zobrazí seznam zakázek"> Zakázky </a></li>
<li><a class="menu" href = ceny.php title="Zobrazí obecný ceník"> Ceník </a></li>
<li><a class="menu" href = edit_user.php title="Editace účtu"> Účet </a></li>
<?php if (file_exists("./doc/main/html/index.html")) { ?>
<li><a class="menu" href = ./doc/main/html/index.html title="Dokumentace k projektu"> Dokumentace </a></li>
<?php } ?>
</ul>
