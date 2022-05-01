<?php
   include('includes/db_connect.php');
   if (isset($_POST["amount"])) { //check for data
      $amount = $_POST['amount']; //изменение количества
      $category_name = $_POST['category_name']; //имя базы данных
      $id = $_POST['id']; //id компонента

      //обновления таблицы
      $sql = "UPDATE `$category_name` SET `amount` = `amount` + $amount WHERE `cell_number` = $id"; //обновление базы данных
      mysqli_query($connection, $sql);

      //вытаскивание нового значения
      $sql = "SELECT `amount` FROM `$category_name` WHERE `cell_number` = $id"; //получение нового значения количества
      $newamount = mysqli_query($connection, $sql);
      $row=mysqli_fetch_assoc($newamount);
      
      //возвращение нового количества
      $result = [ 
            'answer' => $row["amount"],
         ]; 
      echo json_encode($result);
   }
?>