<?php
    require "includes/db_connect.php";

    if(isset($_GET['id'])) {
        $cat_id = $_GET['id'];
    } elseif(isset($_POST['id'])) {
        $cat_id = $_POST['id'];
    } else {
        echo 'Категория компонентов не выбрана'; exit;
    }

    if(isset($_GET['sort_by'])) {
        $sort_var = $_GET['sort_by']; //переменная со значением, по которому будет отсортированна таблица
    } else {
        $sort_var = 0; 
    }

    if(isset($_GET['direction'])) {
        $direction = $_GET['direction']; //переменная, которая хранит направление сортировки таблицы
    } else {

        $direction = 0;
    }

    if(isset($_POST['delete'])) {
        $delete_id = $_POST['delete']; 
    } else {
        $delete_id = 0;
    }
?>

<!doctype html>

<html>
    <head>
        <script src="jquery-3.6.0.min.js"></script> <!-- библиотека для JS -->
        <script src="ajax.js"></script> <!-- файл для динамической работы страницы -->

        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <!-- Bootstrap CSS -->
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">

        <?php
            /* ЗАПРОС НА ПОДКЛЮЧЕНИЕ К ТАБЛИЦЕ С КАТЕГОРИЯМИ КОМПОНЕНТОВ */
            $sql = 'SELECT * FROM `components_categories`';
            $query = mysqli_query($connection, $sql); //спец. тип данных, ссылка на результаты запросы (для SELECT)
            while($cat = mysqli_fetch_assoc($query)) { 
                if($cat_id == $cat['id']) {
                    break;
                }
            }
        ?>
        <title>
            <?php echo $config[$cat['category_name']][$cat['category_name']]; ?>
        </title>
    </head>

    <body>
        <!-- Bootstrap в связке с Popper -->
        <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-/bQdsTh/da6pkI1MST/rWKFNjaCP5gBSY4sEBT38Q/9RBh9AH40zEOg7Hlq2THRZ" crossorigin="anonymous"></script>

        <main>
            <div class="container">

            <?php
                /* ЗАПРОС НА ПОДКЛЮЧЕНИЕ К ТАБЛИЦЕ С КАТЕГОРИЯМИ КОМПОНЕНТОВ */
                $sql = 'SELECT * FROM `components_categories`';
                $query = mysqli_query($connection, $sql); //спец. тип данных, ссылка на результаты запросы (для SELECT)

                while($cat = mysqli_fetch_assoc($query)) //функция возвращет данные в виде ассоциативного массива
                {
                    if($cat_id == $cat['id']) //условие для работы с таблицей, id которой мы передали в GET
                    {
                        echo '<h2 class="mt-4">' . $config[$cat['category_name']][$cat['category_name']] . '</h2>';

                        echo '<p><a href="/index.php" class="btn btn-outline-primary btn-sm">На главную страницу</a></p>';

                        $sql = "SHOW COLUMNS FROM `$cat[category_name]`"; //запрос на получение имен столбцов из выбранной таблицы
                        $result_q = mysqli_query($connection, $sql);
                        
                        $counter = 0; //переменная для подсчёта кол-ва столбцов таблицы
                        $result = array(); //массив, в котором хранятся названия стобцов таблицы
                        
                        while($record = mysqli_fetch_assoc($result_q))
                        {
                            $result[$counter] = $record['Field'];
                            $counter++;
                        }
                        $counter = 10;

            ?>

            <!-- Форма сортировки компонентов -->
            <p>
                <div class="row row-cols-auto">
                    <div class="col">
                        <form action="/category.php" method="get">
                            <input type="hidden" value="<?php echo $cat_id; ?>" name="id">
                            <div class="input-group">
                                <select size="1" name="sort_by" class="form-select" id="inputGroupSelect04">
                                <?php
                                    // Вывод имен столбцов в выпадающий список
                                    for($i = 0; $i < $counter; $i++){
                                        if($result[$i] == $sort_var)
                                        {
                                            echo '<option selected value="' . $result[$i] . '">' . $config[$cat['category_name']][$result[$i]] . '</option>';
                                        }
                                        else
                                        {
                                            echo '<option value="' . $result[$i] . '">' . $config[$cat['category_name']][$result[$i]] . '</option>';
                                        }

                                    }
                                ?>
                                </select>
                                <button  class="btn btn-outline-secondary" type="submit" value="sort increase" name="direction">По возрастанию</button>
                                <button  class="btn btn-outline-secondary" type="submit" value="sort decrease" name="direction">По убыванию</button>
                            </div>
                        </form>
                    </div>
                </div>
            </p>

            <div id="result_form"></div> <!-- форма, в которую возвращаются результаты работы кнопок -->

            <?php
                // Удаление компонента из таблицы
                $sql_d = "DELETE FROM `$cat[category_name]` WHERE `id` ='$delete_id'";
                $result_d = mysqli_query($connection, $sql_d);
            ?>

            <!-- Таблица компонентов -->
            <table class="table table-striped table-hover">
                <thead>
                    <tr class="table-light">
                        <?php
                            // Вывод заголовков таблицы из массива                
                            for($i = 0; $i < $counter; $i++) 
                            {
                                echo '<th>';
                                echo $config[$cat['category_name']][$result[$i]];
                                //echo $result[$i];
                                echo '</th>';
                            }
                            echo "<th>$config[active_buttons]</th>";
                            ?>
                    </tr>
                </thead>
                <tbody>
                    <?php

                        /* УСЛОВИЯ ДЛЯ ЗАПРОСА ВЫВОДА ТАБЛИЦЫ: С СОРТИРОВКОЙ И БЕЗ И В КАКОМ ПОРЯДКЕ */
                        if($sort_var == 0) 
                        {
                            $sql = "SELECT * FROM `$cat[category_name]`";
                        }
                        else
                        {
                            if($direction == 'sort increase')
                            {
                                $sql = "SELECT * FROM `$cat[category_name]` ORDER BY `$sort_var`";
                            }
                            if($direction == 'sort decrease')
                            {
                                $sql = "SELECT * FROM `$cat[category_name]` ORDER BY `$sort_var` DESC";
                            }
                        }
                        
                        $result_q = mysqli_query($connection, $sql);

                        /* ВЫВОД СТРОК ИЗ ТАБЛИЦЫ БД В ТАБЛИЦУ НА ЭКРАНЕ */
                        $cou = 1; // счетчик строк
                        while($record = mysqli_fetch_array($result_q))
                        {
                            echo '<tr>';
                            
                            for($i = 0; $i < $counter; $i++) 
                            {
                                if($result[$i] == 'url')
                                {
                                    echo '<td><a href="' . $record[$i] . '" target="_blank" class="btn btn-outline-primary btn-sm">Ссылка    </a></td>';
                                }
                                else if($result[$i]== 'amount')
                                {
                                    echo '<td><div id="amountdiv'. $cou .'">' . $record[$i] . '</div></td>'; //для количества сделал блок, в который будут добавляться новые значения при увеличении и уменьшении
                                } 
                                else 
                                {
                                    echo '<td>' . $record[$i] . '</td>';
                                }
                            }
                    ?>

                    <td>
                        <form name="active_buttons" id="send<?php echo $cou; ?>" method="post" action="">                    
                            <!-- Кнопка подсветки ячейки -->
                            <p>
                                <input type="hidden" value="<?php echo $record['boardNumber']; ?>" name="boardNumber" id="boardNumber">
                                <input type="hidden" value="<?php echo $record['cell_number']; ?>" name="cell_number" id="cell_number">
                                <input type="hidden" value="<?php echo $record['R']; ?>" name="R" id="R">
                                <input type="hidden" value="<?php echo $record['G']; ?>" name="G" id="G">
                                <input type="hidden" value="<?php echo $record['B']; ?>" name="B" id="B">
                                <input type="hidden" value="<?php echo $record['id']; ?>" name="id" id="id">
                                <button class="btn btn-outline-primary btn-sm" type="button" id="btn<?php echo $cou; ?>" value="Подсветить ячейку №<?php echo $record['cell_number']; ?> " onclick="sendAjaxForm('result_form', 'send<?php echo $cou; ?>', 'send.php');"> <!-- кнопка подстветки ячейки, которая при нажатии запускает JS скрипт -->
                                    Подсветить №<?php echo $record['cell_number']; ?> 
                                </button>
                            </p>
                            <!-- Кнопка изменения колличества компонентов -->
                            <p>
                                <input type="hidden" value="<?php echo $cat['category_name']; ?>" name="category_name" id="category_name">
                                <div class="input-group input-group-sm mb-3">
                                    <input type="number" class="form-control" name="amount" value="1" id="amount" style="width: 50px">
                                    <button class="btn btn-outline-secondary" type="button" value="add amount" onclick="sendAjaxForm('amountdiv<?php echo $cou; ?>', 'send<?php echo $cou; ?>', 'add.php');"    > <!-- кнопка изменения числа ячеек, которая при нажатии запускает JS скрипт -->
                                        Добавить
                                    </button>
                                </div>
                            </p>

                        </form>

                        <!-- Кнопка-триггер модального окна -->
                        <button type="button" class="btn btn-outline-danger btn-sm" data-bs-toggle="modal" data-bs-target="#exampleModal-<?php echo $record['id']; ?>">
                                Удалить
                        </button>

                        <?php
                            if(($direction == 'sort increase') && ($sort_var != 0)) {
                                $action_path = "/category.php?id=$cat_id&sort_by=$sort_var&direction=sort+increase";
                            } elseif(($direction == 'sort decrease') && ($sort_var != 0)) {
                                $action_path = "/category.php?id=$cat_id&sort_by=$sort_var&direction=sort+decrease";
                            } else {
                                $action_path = "/category.php";
                            }
                        ?>

                        <form name="delete_item" action="<?php echo $action_path; ?>" method="post" >
                            <!-- Модальное окно -->
                            <div class="modal fade" id="exampleModal-<?php echo $record['id']; ?>" tabindex="-1" aria-labelledby="exampleModalLabel" aria-hidden="true">
                                <div class="modal-dialog">
                                    <div class="modal-content">
                                        <div class="modal-header">
                                            <h5 class="modal-title" id="exampleModalLabel">Удалить <b><?php echo $record['part_number']; ?></b> из базы данных?</h5>
                                            <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Закрыть"></button>
                                        </div>
                                        <div class="modal-footer">
                                            <input type="hidden" value="<?php echo $cat_id; ?>" name="id">
                                            <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Отмена</button>
                                            <button type="submit" class="btn btn-outline-danger" name="delete" value="<?php echo $record['id']; ?>">Удалить</button>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </form>
                    </td>

                    <?php
                            echo '</tr>';
                            $cou++;
                        }

                    ?>

                </tbody>
            </table>
                
            <?php
                    }
                }
                mysqli_close($connection);
            ?>

            </div>
        </main>
    </body>
</html>

