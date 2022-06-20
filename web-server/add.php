<?php
    require "./includes/db_connect.php";
    require "./includes/config.php";
   if (isset($_POST["amount"])) { //check for data
      $amount = $_POST['amount']; //изменение количества
      $category_name = $_POST['category_name']; //имя базы данных
      $id = $_POST['id']; //id компонента

      //обновления таблицы

       $connection = mysqli_connect(
           $config['db']['server'],
           $config['db']['username'],
           $config['db']['password'],
           $config['db']['name']
       );


      $sql = "UPDATE `$category_name` SET `amount` = `amount` + $amount WHERE `id` = $id"; //обновление базы данных
      mysqli_query($connection, $sql);

      //вытаскивание нового значения
      $sql = "SELECT `amount` FROM `$category_name` WHERE `id` = $id"; //получение нового значения количества
      $newamount = mysqli_query($connection, $sql);
      $row=mysqli_fetch_assoc($newamount);

      //возвращение нового количества
      $result = [
            'answer' => $row["amount"],
         ];

      echo json_encode($result);
   }
?>