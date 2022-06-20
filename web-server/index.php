<?php
    require "includes/db_connect.php";
    require "includes/config.php";
    include_once "./vendor/autoload.php";
?>

<!doctype html>
<html>
    <head>
        <script src="jquery-3.6.0.min.js"></script> <!-- библиотека для JS -->
        <script src="ajax.js"></script> <!-- файл для динамической работы страницы -->
        <script type="text/javascript"> //скрипт обработки динамического импорта файлов
            function hideBtn(){
                $('#upload').hide();
                $('#respondparse').html("Идет загрузка файла");
            }

            function handleResponse(mes) {
                //document.write("check");
                $('#upload').show();

                if (mes.errors != null) {
                    $('#respondparse').html("Возникли ошибки во время загрузки файла: " + mes.errors);
                }   
                else {
                    $('#respondparse').html("Файл " + mes.name + " загружен");   
                }   
            }
        </script>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <!-- Bootstrap CSS -->
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">
        
        <link rel="canonical" href="https://getbootstrap.com/docs/5.1/examples/starter-template/">

        <style>
        .bd-placeholder-img {
            font-size: 1.125rem;
            text-anchor: middle;
            -webkit-user-select: none;
            -moz-user-select: none;
            user-select: none;
        }

        @media (min-width: 768px) {
            .bd-placeholder-img-lg {
                font-size: 3.5rem;
            }
        }
        </style>

        <!-- Custom styles for this template -->
        <link href="starter-template.css" rel="stylesheet">

        <title>Управление учетом электронных компонентов</title>
    </head>

    <body>
        <!-- Bootstrap в связке с Popper -->
        <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-/bQdsTh/da6pkI1MST/rWKFNjaCP5gBSY4sEBT38Q/9RBh9AH40zEOg7Hlq2THRZ" crossorigin="anonymous"></script>
        
        <div class="col-lg-8 mx-auto p-3 py-md-5">
            <main>
                <h1>Управление учетом электронных компонентов</h1>
                <p class="fs-5 col-md-8">Cистема автоматического поиска комплектующих на рабочих местах инженеров-электронщиков</p>
                <hr class="col-3 col-md-2 mb-5">
                <div class="row g-5">
                <div class="col-md-6">
                    <h2>Категории компонентов</h2>
                    <p>Выберите категорию, чтобы перейти к таблице электронных компонентов.</p>
                    <?php
                        $sql = 'SELECT * FROM `components_categories`';
                        $result = mysqli_query($connection, $sql); //спец. тип данных, ссылка на результаты запросы (для SELECT)

                        if($result == false) {
                        echo 'Произошла ошибка';
                        mysqli_error($connection);
                        }

                    ?>

                    <ul class="icon-list">
                        <?php
                            while ($cat = mysqli_fetch_assoc($result)) //функция возвращет данные в виде ассоциативного массива
                            {
                        ?>
                                <li><a href="/category.php?id=<?php echo $cat['id']; ?>" class="btn btn-outline-primary btn-sm"><?php echo $config[$cat['category_name']][$cat['category_name']]; ?></a></li>
                        <?php
                            }
                        ?>
                    </ul>

                </div>

                <?php
                    $sql = 'SELECT * FROM `components_categories`';
                    $result = mysqli_query($connection, $sql); //спец. тип данных, ссылка на результаты запросы (для SELECT)
                ?>

                <div class="col-md-6">
                    <h2>Импорт компонентов в базу данных</h2>
                    <p>Загрузите файл в расширении .csv для добавления компонентов в базу данных. Файл .csv должен соответствовать определённому формат. Формат файлов задаётся администратором.</p>
                    <div>
                        <div class="bd-example">
                            <form action="file_parse.php" method="post" enctype="multipart/form-data" id="parse" name="parse" target="hiddenframe" onsubmit="hideBtn();">  <!-- old action /file_parse.php -->
                                 <div class="mb-3">
                                    <label class="form-label">Выберите категорию</label>
                                    <select class="form-select" name="cat_id" id="cat_id">
                                    <?php
                                        while($cat = mysqli_fetch_assoc($result)) { 
                                    ?>
                                        <option value="<?php echo $cat['category_name'] ?>"><?php echo $config[$cat['category_name']][$cat['category_name']]; ?></option>
                                    <?php
                                        }
                                    ?>
                                    </select>
                                </div>
                                <div class="mb-3">
                                    <input type="file" class="form-control" name="uploadfile" id="uploadfile">
                                </div>
                                <button type="submit" class="btn btn-primary" name="upload" id="upload">Загрузить</button> 
                            </form>
                        </div>
                    </div>

                    <div id="respondparse"></div>

                    <iframe id="hiddenframe" name="hiddenframe" style="width:0px; height:0px; border:0px"></iframe>
                </div>


<!--                Форма добавления новой категории компонентов-->

                    <form action="/new_category.php" method="post" enctype="multipart/form-data">
                        <h2>Добавление новой категории</h2>
                        <div class="form-group">
                            <label class="form-label">Название новой категории компонентов (на русском)</label>
                        <input class="form-control" type="text" size="40" name="new_cat_ru">
                        </p>
                                <label class="form-label">Название новой категории компонентов (на английском, без пробелов)</label>
                        <input class="form-control" type="text" size="40" name="new_cat">
                        </p>
                        <p>
                        <input class="form-control" type="file" name="uploadfile">
                            <p>
                        <input class="btn btn-primary" type="submit" value="Добавить">
                        </p>
                        </div>
                    </form>


                    <div id="result_form"></div>
                    <form action="/new_component.php" method="post" enctype="multipart/form-data">
                        <h2>Добавление нового компонента</h2>

                        <div class="form-group">
                            <button class="btn btn-primary" type="button" id="rfid" value="Добавить из RFID" onclick="sendAjaxForm('result_form', 'rfid', 'rfid.php');">
                                Добавить из RFID-метки
                            </button>
                            <br>
                            <label class="form-label">Выберите категорию</label>
                            <select class="form-select" name="cat_id" id="cat_id">
                                <?php
                                while($cat = mysqli_fetch_assoc($result)) {
                                    ?>
                                    <option value="<?php echo $cat['category_name'] ?>"><?php echo $config[$cat['category_name']][$cat['category_name']]; ?></option>
                                    <?php
                                }
                                ?>
                            </select>
                            <label class="form-label">Наименование компонента</label>
                            <input class="form-control" type="text" size="40" name="new_cat_ru">

                            <label class="form-label">Номер ячейки</label>
                            <input class="form-control" type="text" size="40" name="new_cat_ru">

                            <label class="form-label">Корпус</label>
                            <input class="form-control" type="text" size="40" name="new_cat_ru">

                            <label class="form-label">Количество</label>
                            <input class="form-control" type="text" size="40" name="new_cat_ru">

                            <label class="form-label">Точность</label>
                            <input class="form-control" type="text" size="40" name="new_cat_ru">

                            <label class="form-label">Ссылка</label>
                            <input class="form-control" type="text" size="40" name="new_cat_ru">
                            <br>
                            <p>
                                <input class="btn btn-primary" type="submit" value="Добавить">
                            </p>


                        </div>
                    </form>



                <?php
                    mysqli_close($connection);
                ?>
                </div>
            </main>

        <footer class="pt-5 my-5 text-muted border-top">
            Кутаев Кирилл ИУ4 BMSTU &middot; 2022
        </footer>
    </body>
</html>