<?php if(!empty($echo)) {?>
  <div>
  <div class="chyba">
  <?php echo_echo ($echo);?>
  </div>
  </div>
<?php } ?>
<div class="vnitrni">
<?php
//$Id: user.tmpl.php 90 2007-08-18 19:27:18Z Mira $
//echo_echo($echo);
begin_form($action);
input_form("Jméno","nick",$nick,8);
psswd_form("Heslo","heslo1",$heslo,8);
psswd_form("Potvrzení hesla","heslo2",$heslo,8);
submit_form("Potvrdit","submit");
end_form();
?>
</div>
