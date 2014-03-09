<?php
//zajistí nestartování session při vkládání do jiné stránky
      
if (!session_is_registered("id")){
session_start(); 
}
if (!isset($_SESSION["id"])):
  include ("login.php");
  
else:
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title> Tisk </title>
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="print" >
    <?php echo $body; ?>
  </div>

</body>
<?php endif; ?>
