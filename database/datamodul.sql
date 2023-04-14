-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Waktu pembuatan: 12 Apr 2023 pada 10.00
-- Versi server: 10.4.28-MariaDB
-- Versi PHP: 8.0.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `myets`
--

-- --------------------------------------------------------

--
-- Struktur dari tabel `datamodul`
--

CREATE TABLE `datamodul` (
  `id` int(11) NOT NULL,
  `tagId` varchar(12) NOT NULL,
  `latitude` varchar(12) NOT NULL,
  `longitude` varchar(12) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data untuk tabel `datamodul`
--

INSERT INTO `datamodul` (`id`, `tagId`, `latitude`, `longitude`, `timestamp`) VALUES
(1, 'd3402b', '-8.073797', '112.173227', '2023-04-12 04:50:35'),
(2, 'd3402b', '-8.073796', '112.173234', '2023-04-12 04:51:54'),
(3, 'd3402b', '-8.073803', '112.173236', '2023-04-12 04:52:09'),
(4, 'd3402b', '-8.073796', '112.173236', '2023-04-12 04:52:17'),
(5, 'd3402b', '-8.073795', '112.173234', '2023-04-12 04:52:21'),
(6, 'd3402b', '-8.073797', '112.173235', '2023-04-12 04:52:28'),
(7, 'd3402b', '-8.073802', '112.173231', '2023-04-12 04:52:33'),
(8, 'd3402b', '-8.073805', '112.173225', '2023-04-12 04:52:44'),
(9, 'd3402b', '-8.073805', '112.173267', '2023-04-12 07:56:16'),
(10, 'd3402b', '-8.073816', '112.173267', '2023-04-12 07:56:50'),
(11, 'd3402b', '-8.073820', '112.173263', '2023-04-12 07:57:42');

--
-- Indexes for dumped tables
--

--
-- Indeks untuk tabel `datamodul`
--
ALTER TABLE `datamodul`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT untuk tabel yang dibuang
--

--
-- AUTO_INCREMENT untuk tabel `datamodul`
--
ALTER TABLE `datamodul`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
