-- phpMyAdmin SQL Dump
-- version 5.1.0
-- https://www.phpmyadmin.net/
--
-- Хост: 127.0.0.1:3306
-- Время создания: Сен 28 2021 г., 00:00
-- Версия сервера: 5.7.33
-- Версия PHP: 8.0.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- База данных: `components_db`
--

-- --------------------------------------------------------

--
-- Структура таблицы `bipolar_transistors`
--

CREATE TABLE `bipolar_transistors` (
  `id` int(11) NOT NULL,
  `cell_number` int(11) NOT NULL,
  `part_number` varchar(100) DEFAULT NULL,
  `package` varchar(100) DEFAULT NULL,
  `structure` varchar(11) NOT NULL,
  `Vcbo_max` float DEFAULT NULL,
  `Vceo_max` float DEFAULT NULL,
  `Ic_max` float DEFAULT NULL,
  `power` float DEFAULT NULL,
  `amount` int(11) NOT NULL,
  `url` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `bipolar_transistors`
--

INSERT INTO `bipolar_transistors` (`id`, `cell_number`, `part_number`, `package`, `structure`, `Vcbo_max`, `Vceo_max`, `Ic_max`, `power`, `amount`, `url`) VALUES
(3, 23, '2SA1941-O(Q)', 'TO-3P', 'PNP', 140, 140, 10, 100, 60, 'https://www.chipdip.ru/product/2sa1941'),
(5, 21, 'КТ3102АМ', 'КТ-26 / TO-92', 'NPN', 50, 50, 0.2, 0.25, 8, 'https://www.chipdip.ru/product/kt3102am-integral'),
(6, 22, '2N3904', 'TO-92 Ammo', 'NPN', 60, 40, 0.2, 0.35, 46, 'https://www.chipdip.ru/product/2n3904');

-- --------------------------------------------------------

--
-- Структура таблицы `components_categories`
--

CREATE TABLE `components_categories` (
  `id` int(11) NOT NULL,
  `category_name` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `components_categories`
--

INSERT INTO `components_categories` (`id`, `category_name`) VALUES
(1, 'resistors'),
(2, 'electrolytic_capacitors'),
(3, 'bipolar_transistors');

-- --------------------------------------------------------

--
-- Структура таблицы `electrolytic_capacitors`
--

CREATE TABLE `electrolytic_capacitors` (
  `id` int(11) NOT NULL,
  `cell_number` int(11) NOT NULL,
  `part_number` varchar(100) DEFAULT NULL,
  `package` varchar(100) DEFAULT NULL,
  `voltage` float DEFAULT NULL,
  `capacitance` float NOT NULL,
  `tolerance` float DEFAULT NULL,
  `amount` int(11) NOT NULL,
  `url` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `electrolytic_capacitors`
--

INSERT INTO `electrolytic_capacitors` (`id`, `cell_number`, `part_number`, `package`, `voltage`, `capacitance`, `tolerance`, `amount`, `url`) VALUES
(3, 33, 'B41851F9108M000', 'К50-35', 100, 1000, 20, 28, 'https://www.chipdip.ru/product0/9000254439'),
(5, 31, 'B43851K5475M000', 'К50-35', 450, 4.7, 20, 100, 'https://www.chipdip.ru/product0/9000631539'),
(8, 32, 'B41851F9108M000', 'К50-35', 100, 20, 20, 28, 'https://www.chipdip.ru/product0/9000254439');

-- --------------------------------------------------------

--
-- Структура таблицы `resistors`
--

CREATE TABLE `resistors` (
  `id` int(11) NOT NULL,
  `cell_number` int(11) NOT NULL,
  `part_number` varchar(100) DEFAULT NULL,
  `package` varchar(100) DEFAULT NULL,
  `resistance` float NOT NULL,
  `tolerance` float NOT NULL,
  `power` float DEFAULT NULL,
  `max_voltage` float DEFAULT NULL,
  `amount` int(11) NOT NULL,
  `url` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `resistors`
--

INSERT INTO `resistors` (`id`, `cell_number`, `part_number`, `package`, `resistance`, `tolerance`, `power`, `max_voltage`, `amount`, `url`) VALUES
(1, 11, 'CF-100 (С1-4)', '	с1-4', 1, 5, 1, 500, 76, 'https://www.chipdip.ru/product0/11677'),
(2, 12, 'KNP-100', '	knp', 0.1, 5, 1, NULL, 8, 'https://www.chipdip.ru/product0/769745805'),
(15, 13, 'KNP-1W-0R1J', '	с2-29в', 1.8, 1, 0.25, 350, 220, 'https://www.chipdip.ru/product0/9000217675');

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `bipolar_transistors`
--
ALTER TABLE `bipolar_transistors`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `components_categories`
--
ALTER TABLE `components_categories`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `electrolytic_capacitors`
--
ALTER TABLE `electrolytic_capacitors`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `resistors`
--
ALTER TABLE `resistors`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `bipolar_transistors`
--
ALTER TABLE `bipolar_transistors`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT для таблицы `components_categories`
--
ALTER TABLE `components_categories`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT для таблицы `electrolytic_capacitors`
--
ALTER TABLE `electrolytic_capacitors`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=60;

--
-- AUTO_INCREMENT для таблицы `resistors`
--
ALTER TABLE `resistors`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=16;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
