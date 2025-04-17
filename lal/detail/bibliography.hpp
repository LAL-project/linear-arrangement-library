/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <string_view>
#include <array>

// lal includes
#include <lal/bibliography.hpp>

namespace lal {
namespace detail {

#if defined LAL_REGISTER_BIBLIOGRAPHY

// clang-format off

/// String for reference \cite Alemany2024b.
static constexpr std::string_view Alemany2024b =
	"@phdthesis{Alemany2024b,\n"
	"    author = {{Alemany-Puig}, {Llu\\'{i}s}},\n"
	"    title = {{Theory, Algorithms and Applications of Arrangements of Graphs: Generation, Expectation and Optimization}},\n"
	"    school = {Universitat Polit\\'ecnica de Catalunya -- Barcelonatech},\n"
	"    year = {2024},\n"
	"    url = {https://hdl.handle.net/10803/693663}\n"
	"}";

/// String for reference \cite Alemany2024a.
static constexpr std::string_view Alemany2024a =
	"@article{Alemany2024a,\n"
	"    title = {{The maximum linear arrangement problem for trees under projectivity and planarity}},\n"
	"    author = {{Alemany-Puig}, {Llu\\'{i}s} and {Esteban}, {Juan Luis} and {Ferrer-i-Cancho}, {Ramon}},\n"
	"    journal = {Information Processing Letters},\n"
	"    year = {2024},\n"
	"    volume = {183},\n"
	"    pages = {106400},\n"
	"    issn = {0020-0190},\n"
	"    doi = {10.1016/j.ipl.2023.106400},\n"
	"    url = {http://arxiv.org/abs/2206.06924},\n"
	"}";

/// String for reference \cite Alemany2023a.
static constexpr std::string_view Alemany2023a =
	"@article{Alemany2023a,\n"
	"    title = {{On The Maximum Linear Arrangement Problem for Trees}},\n"
	"    author = {{Alemany-Puig}, Llu\\'{i}s and Esteban, {Juan Luis} and {Ferrer-i-Cancho}, Ramon},\n"
	"    journal = {arXiv},\n"
	"    arxivId = {2312.04487},\n"
	"    url = {https://arxiv.org/abs/2312.04487},\n"
	"    year = {2023}\n"
	"}";

/// String for reference \cite Alemany2022a.
static constexpr std::string_view Alemany2022a =
	"@article{Alemany2022a,\n"
	"    title = {{Minimum projective linearizations of trees in linear time}},\n"
	"    journal = {Information Processing Letters},\n"
	"    author = {{Alemany-Puig}, Llu\\'{i}s and Esteban, {Juan Luis} and {Ferrer-i-Cancho}, Ramon},\n"
	"    year = {2022},\n"
	"    volume = {174},\n"
	"    pages = {106204},\n"
	"    issn = {0020-0190},\n"
	"    doi = {10.1016/j.ipl.2021.106204},\n"
	"    url = {http://arxiv.org/abs/2102.03277}\n"
	"}";

/// String for reference \cite Alemany2022b.
static constexpr std::string_view Alemany2022b =
	"@article{Alemany2022b,\n"
	"    title = {{Linear-time calculation of the expected sum of edge lengths in projective linearizations of trees}},\n"
	"    author = {{Alemany-Puig}, Llu\\'{i}s and {Ferrer-i-Cancho}, Ramon},\n"
	"    journal = {Computational Linguistics},\n"
	"    month = {04},\n"
	"    year = {2022},\n"
	"    volume = {48},\n"
	"    number = {3},\n"
	"    pages = {491-516},\n"
	"    issn = {0891-2017},\n"
	"    doi = {10.1162/coli_a_00442},\n"
	"    url = {https://arxiv.org/abs/2107.03277}\n"
	"}";

/// String for reference \cite Alemany2022c.
static constexpr std::string_view Alemany2022c =
	"@article{Alemany2022c,\n"
	"    title = {{Linear-time calculation of the expected sum of edge lengths in planar linearizations of trees}},\n"
	"    author = {{Alemany-Puig}, Llu\\'{i}s and {Ferrer-i-Cancho}, Ramon},\n"
	"    archivePrefix = {arXiv},\n"
	"    journal = {Arxiv},\n"
	"    arxivId = {2207.05564},\n"
	"    eprint = {2207.05564},\n"
	"    url = {http://arxiv.org/abs/2207.05564},\n"
	"    year = {2022}\n"
	"}";

/// String for reference \cite Anderson2021a.
static constexpr std::string_view Anderson2021a =
	"@phdthesis{Anderson2021a,\n"
	"    author = {Anderson, Mark},\n"
	"    title = {{An Unsolicited Soliloquy on Dependency Parsing}},\n"
	"    school = {Universidade da Coru\\~na},\n"
	"    year = {2021},\n"
	"    url = {https://ruc.udc.es/dspace/bitstream/handle/2183/28513/Anderson_Mark_TD_2021.pdf?sequence=1}\n"
	"}";

/// String for reference \cite Macutek2021a.
static constexpr std::string_view Macutek2021a =
	"@inproceedings{Macutek2021a,\n"
	"    title = {{The Menzerath-Altmann law in syntactic structure revisited}},\n"
	"    author = {Ma{\\v{c}}utek, J{\\'a}n and {\\v{C}}ech, Radek and Courtin, Marine},\n"
	"    booktitle = {Proceedings of the Second Workshop on Quantitative Syntax (Quasy, SyntaxFest 2021)},\n"
	"    month = 12,\n"
	"    year = {2021},\n"
	"    address = {Sofia, Bulgaria},\n"
	"    publisher = {Association for Computational Linguistics},\n"
	"    url = {https://aclanthology.org/2021.quasy-1.6},\n"
	"    pages = {65--73}\n"
	"}";

/// String for reference \cite Alemany2020a.
static constexpr std::string_view Alemany2020a =
	"@article{Alemany2020a,\n"
	"    title = {{Edge crossings in random linear arrangements}},\n"
	"    author = {{Alemany-Puig}, Llu\\'{i}s and {Ferrer-i-Cancho}, Ramon},\n"
	"    journal = {Journal of Statistichal Mechanics},\n"
	"    issue = {2},\n"
	"    pages = {023403},\n"
	"    volume = {2020},\n"
	"    year = {2020},\n"
	"    doi = {10.1088/1742-5468/ab6845},\n"
	"    url = {https://arxiv.org/abs/1910.03926}\n"
	"}";

/// String for reference \cite Alemany2020b.
static constexpr std::string_view Alemany2020b =
	"@article{Alemany2020b,\n"
	"    title = {{Fast calculation of the variance of edge crossings}},\n"
	"    author = {{Alemany-Puig}, Llu\\'{i}s and {Ferrer-i-Cancho}, Ramon},\n"
	"    archivePrefix = {arXiv},\n"
	"    journal = {arXiv},\n"
	"    arxivId = {2003.03258},\n"
	"    eprint = {2003.03258},\n"
	"    url = {http://arxiv.org/abs/2003.03258},\n"
	"    year = {2021}\n"
	"}";

/// String for reference \cite Johnson2020a.
static constexpr std::string_view Johnson2020a =
	"@article{Johnson2020a,\n"
	"    title = {{The proportion of trees that are linear}},\n"
	"    author = {Wakhare, Tanay and Wityk, Eric and Johnson, {Charles R.}},\n"
	"    journal = {Discrete Mathematics},\n"
	"    volume = {343},\n"
	"    number = {10},\n"
	"    pages = {112008},\n"
	"    year = {2020},\n"
	"    issn = {0012-365X},\n"
	"    doi = {10.1016/j.disc.2020.112008},\n"
	"    url = {https://www.sciencedirect.com/science/article/pii/S0012365X20301941}\n"
	"}";

/// String for reference \cite Alemany2019a.
static constexpr std::string_view Alemany2019a =
	"@MastersThesis{Alemany2019a,\n"
	"    title = {{Edge crossings in linear arrangements: from theory to algorithms and applications.}},\n"
	"    author = {{Alemany-Puig}, Llu\\'{i}s},\n"
	"    year = {2019},\n"
	"    type = {{Master Thesis (M. Sc.)}},\n"
	"    location = {Barcelona, Spain},\n"
	"    institution = {Universitat Polit\\`ecnica de Catalunya - BarcelonaTech (UPC)},\n"
	"    publisher = {Universitat Polit\\`ecnica de Catalunya - BarcelonaTech (UPC)},\n"
	"    url = {https://upcommons.upc.edu/handle/2117/168124}\n"
	"}";

/// String for reference \cite Anderson2019a.
static constexpr std::string_view Anderson2019a =
	"@inproceedings{Anderson2019a,\n"
	"    title = {{Artificially Evolved Chunks for Morphosyntactic Analysis}},\n"
	"    author = {Anderson, Mark and Vilares, David and {G\\'{o}mez-Rodr\\'{i}guez}, Carlos},\n"
	"    booktitle = {Proceedings of the 18th International Workshop on Treebanks and Linguistic Theories (TLT, SyntaxFest 2019)},\n"
	"    month = 08,\n"
	"    year = {2019},\n"
	"    address = {Paris, France},\n"
	"    publisher = {Association for Computational Linguistics},\n"
	"    url = {https://aclanthology.org/W19-7815},\n"
	"    doi = {10.18653/v1/W19-7815},\n"
	"    pages = {133--143},\n"
	"}";

/// String for reference \cite English2019a.
static constexpr std::string_view English2019a =
	"@article{English2019a,\n"
	"    title = {{Weighted Tur\\'{a}n problems with applications}},\n"
	"    author = {Bennett, Patrick and English, Sean and {Talanda-Fisher}, Maria},\n"
	"    journal = {Discrete Mathematics},\n"
	"    doi = {10.1016/j.disc.2019.04.007},\n"
	"    issn = {0012365X},\n"
	"    issue = {8},\n"
	"    month = {8},\n"
	"    pages = {2165-2172},\n"
	"    publisher = {Elsevier B.V.},\n"
	"    volume = {342},\n"
	"    year = {2019}\n"
	"}";

/// String for reference \cite Ferrer2019a.
static constexpr std::string_view Ferrer2019a =
	"@article{Ferrer2019a,\n"
	"    title = {{The sum of edge lengths in random linear arrangements}},\n"
	"    author = {{Ferrer-i-Cancho}, Ramon},\n"
	"    journal = {Journal of Statistichal Mechanics},\n"
	"    number = {5},\n"
	"    pages = {053401},\n"
	"    publisher = {IOP Publishing},\n"
	"    volume = {2019},\n"
	"    year = {2019},\n"
	"    doi = {10.1088/1742-5468/ab11e2}\n"
	"}";

/// String for reference \cite Nurse2019a.
static constexpr std::string_view Nurse2019a =
	"@mastersthesis{Nurse2019a,\n"
	"    title = {{Maximum linear arrangement of directed graphs}},\n"
	"    author = {Nurse, Kathryn},\n"
	"    school = {Simon Fraser University},\n"
	"    pages = {59},\n"
	"    year = {2019},\n"
	"    url = {https://summit.sfu.ca/item/19406}\n"
	"}";

/// String for reference \cite Ferrer2018a.
static constexpr std::string_view Ferrer2018a =
	"@article{Ferrer2018a,\n"
	"    title = {{Are crossing dependencies really scarce?}},\n"
	"    author = {{Ferrer-i-Cancho}, Ramon and {G\\'{o}mez-Rodr\\'{i}guez}, Carlos and Esteban, {Juan Luis}},\n"
	"    journal = {Physica A: Statistical Mechanics and its Applications},\n"
	"    volume = {493},\n"
	"    pages = {311-329},\n"
	"    year = {2018},\n"
	"    doi = {10.1016/j.physa.2017.10.048}\n"
	"}";

/// String for reference \cite Nurse2018a.
static constexpr std::string_view Nurse2018a =
	"@article{Nurse2018a,\n"
	"    title = {{A Maximum Linear Arrangement Problem on Directed Graphs}},\n"
	"    journal = {arXiv},\n"
	"    author = {DeVos, Matt and Nurse, Kathryn},\n"
	"    year = {2018},\n"
	"    eprint = {1810.12277},\n"
	"    archivePrefix = {arXiv},\n"
	"    primaryClass = {cs.DS}\n"
	"}";

/// String for reference \cite Esteban2017a.
static constexpr std::string_view Esteban2017a =
	"@article{Esteban2017a,\n"
	"    title = {{A Correction on Shiloach's Algorithm for Minimum Linear Arrangement of Trees}},\n"
	"    author = {Esteban, {Juan Luis} and {Ferrer-i-Cancho}, Ramon},\n"
	"    journal = {SIAM Journal on Computing},\n"
	"    volume = {46},\n"
	"    number = {3},\n"
	"    pages = {1146-1151},\n"
	"    year = {2017},\n"
	"    doi = {10.1137/15M1046289},\n"
	"    url = {https://arxiv.org/abs/1511.01061}\n"
	"}";

/// String for reference \cite Kahane2017a.
static constexpr std::string_view Kahane2017a =
	"@inproceedings{Kahane2017a,\n"
	"    title = {{What are the limitations on the flux of syntactic dependencies? Evidence from UD treebanks}},\n"
	"    author = {Kahane, Sylvain and Yan, Chunxiao and Botalla, {Marie-Am\\'{e}lie}},\n"
	"    city = {Pisa, Italy},\n"
	"    booktitle = {Proceedings of the Fourth International Conference on Dependency Linguistics},\n"
	"    month = {9},\n"
	"    pages = {73-82},\n"
	"    url = {https://aclanthology.org/W17-6510},\n"
	"    year = {2017},\n"
	"    HAL_ID = {hal-01675325}\n"
	"}";

/// String for reference \cite Futrell2015a.
static constexpr std::string_view Futrell2015a =
	"@article{Futrell2015a,\n"
	"    title = {{Large-scale evidence of dependency length minimization in 37 languages}},\n"
	"    author = {Futrell, Richard and Mahowald, Kyle and Gibson, Edward},\n"
	"    journal = {Proceedings of the National Academy of Sciences},\n"
	"    volume = {112},\n"
	"    number = {33},\n"
	"    pages = {10336--10341},\n"
	"    year = {2015},\n"
	"    doi = {10.1073/pnas.1502134112}\n"
	"}";

/// String for reference \cite Jing2015a.
static constexpr std::string_view Jing2015a =
	"@inproceedings{Jing2015a,\n"
	"    title = {{Mean hierarchical distance. Augmenting mean dependency distance}},\n"
	"    author = {Jing, Yingqi and Liu, Haitao},\n"
	"    year = {2015},\n"
	"    booktitle = {Proceedings of the Third International Conference on Dependency Linguistics},\n"
	"    pages = {161-170}\n"
	"}";

/// String for reference \cite Ferrer2014a.
static constexpr std::string_view Ferrer2014a =
	"@article{Ferrer2014a,\n"
	"    title = {{A stronger null hypothesis for crossing dependencies}},\n"
	"    author = {{Ferrer-i-Cancho}, Ramon},\n"
	"    doi = {10.1209/0295-5075/108/58003},\n"
	"    year = {2014},\n"
	"    month = {12},\n"
	"    publisher = {{IOP} Publishing},\n"
	"    volume = {108},\n"
	"    number = {5},\n"
	"    pages = {58003},\n"
	"    journal = {{EPL} (Europhysics Letters)}\n"
	"}";

/// String for reference \cite Pitler2013a.
static constexpr std::string_view Pitler2013a =
	"@inproceedings{Pitler2013a,\n"
	"    title = {{Finding Optimal 1-Endpoint-Crossing Trees}},\n"
	"    author = {Satta, Giorgio and Pitler, Emily and Kannan, Sampath and Marcus, Mitchell},\n"
	"    booktitle = {Transactions of the Association for Computational Linguistics},\n"
	"    pages = {13-24},\n"
	"    url = {https://www.mitpressjournals.org/doi/pdfplus/10.1162/tacl_a_00206},\n"
	"    year = {2013}\n"
	"}";

/// String for reference \cite Gomez2011a.
static constexpr std::string_view Gomez2011a =
	"@article{Gomez2011a,\n"
	"    title = {{Dependency Parsing Schemata and Mildly Non-Projective Dependency Parsing}},\n"
	"    author = {{G\\'{o}mez-Rodr\\'{i}guez}, Carlos and Carroll, John and Weir, David},\n"
	"    journal = {Computational Linguistics},\n"
	"    number = {3},\n"
	"    pages = {541--586},\n"
	"    volume = {37},\n"
	"    year = {2011},\n"
	"    doi = {10.1162/COLI_a_00060}\n"
	"}";

/// String for reference \cite Liu2010a.
static constexpr std::string_view Liu2010a =
	"@article{Liu2010a,\n"
	"    title = {{Dependency direction as a means of word-order typology: a method based on dependency treebanks}},\n"
	"    author = {Liu, Haitao},\n"
	"    journal = {Lingua},\n"
	"    year = {2010},\n"
	"    volume = {120},\n"
	"    number = {6},  \n"
	"    pages = {1567-1578}\n"
	"}";

/// String for reference \cite Gildea2007a.
static constexpr std::string_view Gildea2007a =
	"@inproceedings{Gildea2007a,\n"
	"    title = {{Optimizing Grammars for Minimum Dependency Length}},\n"
	"    author = {Gildea, Daniel and Temperley, David},\n"
	"    booktitle = {Proceedings of the 45th Annual Meeting of the Association of Computational Linguistics},\n"
	"    month = {06},\n"
	"    year = {2007},\n"
	"    address = {Prague, Czech Republic},\n"
	"    publisher = {Association for Computational Linguistics},\n"
	"    url = {https://www.aclweb.org/anthology/P07-1024},\n"
	"    pages = {184--191}\n"
	"}";

/// String for reference \cite Ferrer2004a.
static constexpr std::string_view Ferrer2004a =
	"@article{Ferrer2004a,\n"
	"    title = {{Euclidean distance between syntactically linked words}},\n"
	"    author = {{Ferrer-i-Cancho}, Ramon},\n"
	"    journal = {Physical Review E},\n"
	"    issn = {1063651X},\n"
	"    number = {5},\n"
	"    pages = {5},\n"
	"    pmid = {15600720},\n"
	"    volume = {70},\n"
	"    year = {2004},\n"
	"    doi = {10.1103/PhysRevE.70.056135}\n"
	"}";

/// String for reference \cite Hochberg2003a.
static constexpr std::string_view Hochberg2003a =
	"@article{Hochberg2003a,\n"
	"    title = {{Optimal one-page tree embeddings in linear time}},\n"
	"    author = {Hochberg, {Robert A.} and Stallmann, {Matthias F.}},\n"
	"    journal = {Information Processing Letters},\n"
	"    volume = {87},\n"
	"    number = {2},\n"
	"    pages = {59--66},\n"
	"    year = {2003},\n"
	"    issn = {0020-0190},\n"
	"    doi = {10.1016/S0020-0190(03)00261-8}\n"
	"}";

/// String for reference \cite Cormen2001a.
static constexpr std::string_view Cormen2001a =
	"@book{Cormen2001a,\n"
	"    title = {{Introduction to Algorithms}},\n"
	"    address = {Cambridge, MA, USA},\n"
	"    author = {Cormen, {Thomas H.} and Leiserson, {Charles E.} and Rivest, {Ronald L.} and Stein, Clifford},\n"
	"    edition = {2nd},\n"
	"    publisher = {The MIT Press},\n"
	"    year = {2001}\n"
	"}";

/// String for reference \cite Iordanskii1987a.
static constexpr std::string_view Iordanskii1987a =
	"@inproceedings{Iordanskii1987a,\n"
	"    title = {{Minimal numberings of the vertices of trees --- Approximate approach}},\n"
	"    author = {Iordanskii, {Mikhail Anatolievich}},\n"
	"    booktitle = {Fundamentals of Computation Theory},\n"
	"    editor = {Budach, Lothar and Bukharajev, Rais Gati\\v{c} and Lupanov, Oleg Borisovi\\v{c}},\n"
	"    year = {1987},\n"
	"    publisher = {Springer Berlin Heidelberg},\n"
	"    address = {Berlin, Heidelberg},\n"
	"    pages = {214--217},\n"
	"    isbn = {978-3-540-48138-6}\n"
	"}";

/// String for reference \cite Wright1986a.
static constexpr std::string_view Wright1986a =
	"@article{Wright1986a,\n"
	"    title = {{Constant Time Generation of Free Trees}},\n"
	"    author = {Wright, Robert Alan and Richmond, Bruce and Odlyzko, Andrew and McKay, {Brendan D.}},\n"
	"    year = {1986},\n"
	"    month = {05},\n"
	"    pages = {540-548},\n"
	"    volume = {15},\n"
	"    journal = {SIAM Journal on Computing},\n"
	"    doi = {10.1137/0215039}\n"
	"}";

/// String for reference \cite Chung1984a.
static constexpr std::string_view Chung1984a =
	"@article{Chung1984a,\n"
	"    title = {{On optimal linear arrangements of trees}},\n"
	"    author = {Chung, {Fan R. K.}},\n"
	"    journal = {Computers \\& Mathematics with Applications},\n"
	"    volume = {10},\n"
	"    number = {1},\n"
	"    pages = {43--60},\n"
	"    year = {1984},\n"
	"    issn = {0898-1221},\n"
	"    doi = {10.1016/0898-1221(84)90085-3}\n"
	"}";

/// String for reference \cite Colbourn1981a.
static constexpr std::string_view Colbourn1981a =
	"@article{Colbourn1981a,\n"
	"    title = {{Linear Time Automorphism Algorithms for Trees, Interval Graphs, and Planar Graphs}},\n"
	"    author = {Colbourn, {Charles J.} and Booth, {Kellogg S.}},\n"
	"    journal = {SIAM Journal on Computing},\n"
	"    volume = {10},\n"
	"    number = {1},\n"
	"    pages = {203-225},\n"
	"    year = {1981},\n"
	"    doi = {10.1137/0210015}\n"
	"}";

/// String for reference \cite Wilf1981a.
static constexpr std::string_view Wilf1981a =
	"@article{Wilf1981a,\n"
	"    title = {{The uniform selection of free trees}},\n"
	"    author = {Wilf, {Herbert S.}},\n"
	"    year = {1981},\n"
	"    journal = {Journal of Algorithms},\n"
	"    issn = {0196-6774},\n"
	"    volume = {2},\n"
	"    issue = {2},\n"
	"    pages = {204-207},\n"
	"    publisher = {Elsevier BV},\n"
	"    doi = {10.1016/0196-6774(81)90021-3}\n"
	"}";

/// String for reference \cite Beyer1980a.
static constexpr std::string_view Beyer1980a =
	"@article{Beyer1980a,\n"
	"    author = {Beyer, Terry and Hedetniemi, {Sandra Mitchell}},\n"
	"    title = {{Constant Time Generation of Rooted Trees}},\n"
	"    journal = {SIAM Journal on Computing},\n"
	"    volume = {9},\n"
	"    number = {4},\n"
	"    pages = {706-712},\n"
	"    year = {1980},\n"
	"    doi = {10.1137/0209055}\n"
	"}";

/// String for reference \cite Shiloach1979a.
static constexpr std::string_view Shiloach1979a =
	"@article{Shiloach1979a,\n"
	"    title = {{A Minimum Linear Arrangement Algorithm for Undirected Trees}},\n"
	"    author = {Shiloach, Yossi},\n"
	"    journal = {SIAM Journal on Computing},\n"
	"    volume = {8},\n"
	"    number = {1},\n"
	"    pages = {15--32},\n"
	"    year = {1979},\n"
	"    doi = {10.1137/0208002}\n"
	"}";

/// String for reference \cite Nijenhuis1978a.
static constexpr std::string_view Nijenhuis1978a =
	"@book{Nijenhuis1978a,\n"
	"    author = {Nijenhuis, Albert and Wilf, {Herbert S.}},\n"
	"    title = {{Combinatorial Algorithms: For Computers and Hard Calculators}},\n"
	"    year = {1978},\n"
	"    isbn = {0125192606},\n"
	"    edition = {2nd},\n"
	"    publisher = {Academic Press, Inc.},\n"
	"    address = {Orlando, FL, USA},\n"
	"}";

/// String for reference \cite Aho1974a.
static constexpr std::string_view Aho1974a =
	"@book{Aho1974a,\n"
	"    title = {{The Design and Analysis of Computer Algorithms}},\n"
	"    author = {Aho, {Alfred V.} and Hopcroft, {Jeffrey E.} and Ullman, {John D.}},\n"
	"    series = {Addison-Wesley series in computer science and information processing},\n"
	"    year = {1974},\n"
	"    isbn = {9780201000290},\n"
	"    edition = {1st},\n"
	"    publisher = {Addison-Wesley Publishing Company},\n"
	"    address = {Michigan University}\n"
	"}";

/// String for reference \cite Harary1973a.
static constexpr std::string_view Harary1973a =
	"@article{Harary1973a,\n"
	"    title = {{The number of caterpillars}},\n"
	"    author = {Harary, Frank and Schwenk, {Allen J.}},\n"
	"    journal = {Discrete Mathematics},\n"
	"    issn = {0012365X},\n"
	"    issue = {4},\n"
	"    pages = {359--365},\n"
	"    volume = {6},\n"
	"    year = {1973},\n"
	"    doi = {10.1016/0012-365X(73)90067-8}\n"
	"}";

/// String for reference \cite Karp1972a.
static constexpr std::string_view Karp1972a =
	"@inbook{Karp1972a,\n"
	"    title = {{Reducibility among Combinatorial Problems}},\n"
	"    author = {Karp, {Richard M.}},\n"
	"    editor = {Miller, {Raymond E.} and Thatcher, {James W.} and Bohlinger, {Jean D.}},\n"
	"    bookTitle = {{Complexity of Computer Computations: Proceedings of a symposium on the Complexity of Computer Computations, held March 20--22, 1972, at the IBM Thomas J. Watson Research Center, Yorktown Heights, New York, and sponsored by the Office of Naval Research, Mathematics Program, IBM World Trade Corporation, and the IBM Research Mathematical Sciences Department}},\n"
	"    year = {1972},\n"
	"    publisher = {Springer US},\n"
	"    address = {Boston, MA},\n"
	"    pages = {85--103},\n"
	"    isbn = {978-1-4684-2001-2},\n"
	"    doi = {10.1007/978-1-4684-2001-2_9}\n"
	"}";

/// String for reference \cite Harary1969a.
static constexpr std::string_view Harary1969a =
	"@book{Harary1969a,\n"
	"    title = {{Graph Theory}},\n"
	"    author = {Harary, Frank},\n"
	"    year = {1969},\n"
	"    edition = {2nd},\n"
	"    publisher = {CRC Press},\n"
	"    address = {Boca Raton, FL, USA},\n"
	"}";

/// String for reference \cite Otter1948a.
static constexpr std::string_view Otter1948a =
	"@article{Otter1948a,\n"
	"    title = {{Annals of Mathematics The Number of Trees}},\n"
	"    author = {Otter, Richard},\n"
	"    number = {3},\n"
	"    pages = {583--599},\n"
	"    volume = {49},\n"
	"    year = {1948},\n"
	"    doi = {10.2307/1969046},\n"
	"}";

/// String for reference \cite Pruefer1918a.
static constexpr std::string_view Pruefer1918a =
	"@article{Pruefer1918a,\n"
	"    title = {{Neuer Beweis eines Satzes \"{u}ber Permutationen}},\n"
	"    author = {Pr\"{u}fer, Heinz},\n"
	"    journal = {Arch. Math. Phys},\n"
	"    volume = {27},\n"
	"    pages = {742-744},\n"
	"    year = {1918}\n"
	"}";

/// String for reference \cite OEIS_A000055.
static constexpr std::string_view OEIS_A000055 =
	"@misc{OEIS_A000055,\n"
	"    author = {Sloane, {N. J. A.}},\n"
	"    title = {{The On-line Encyclopedia of Integer Sequences - A000055 - Number of trees with n unlabeled nodes.}},\n"
	"    howpublished = {\\url{https://oeis.org/A000055}},\n"
	"    note = {Accessed: 2019-12-28}\n"
	"}";

/// String for reference \cite OEIS_A000081.
static constexpr std::string_view OEIS_A000081 =
	"@misc{OEIS_A000081,\n"
	"    author = {Sloane, {N. J. A.}},\n"
	"    title = {{The On-line Encyclopedia of Integer Sequences - A000081 - Number of unlabeled rooted trees with n nodes (or connected functions with a fixed point)}},\n"
	"    howpublished = {\\url{https://oeis.org/A000081}},\n"
	"    note = {Accessed: 2019-03-31}\n"
	"}";

/// String for reference \cite OEIS_kLinearTrees.
static constexpr std::string_view OEIS_kLinearTrees =
	"@misc{OEIS_kLinearTrees,\n"
	"    title = {{The On-Line Encyclopedia of Integer Sequences - A338706 - Number of $2$-linear trees on $n$ nodes.}},\n"
	"    author = {Sloane, {Neil James Alexander}},\n"
	"    year = {1964-2022},\n"
	"    howpublished = {\\url{https://oeis.org/A338706}},\n"
	"    note = {Accessed: 2022-10-07}\n"
	"}";

/// String for reference \cite GMP_library.
static constexpr std::string_view GMP_library =
	"@misc{GMP_library,\n"
	"    title = {{The GNU Multiple Precision Arithmetic Library}},\n"
	"    howpublished = {\\url{https://gmplib.org/}},\n"
	"    note = {Accessed: 2020-03-24}\n"
	"}";

/// String for reference \cite MathWorld_SpiderGraph.
static constexpr std::string_view MathWorld_SpiderGraph =
	"@misc{MathWorld_SpiderGraph,\n"
	"    title = {{Spider Graph -- Wolfram Math World}},\n"
	"    howpublished = {\\url{https://mathworld.wolfram.com/SpiderGraph.html}},\n"
	"    note = {Accessed: 2022-10-04}\n"
	"}";

/// String for reference \cite GiacXcas_Manual.
static constexpr std::string_view GiacXcas_Manual =
	"@misc{GiacXcas_Manual,\n"
	"    author = {Marohnić, Luka},\n"
	"    year = {2018},\n"
	"    title = {{Graph theory package for Giac/Xcas - Reference manual}},\n"
	"    howpublished = {\\url{ https://usermanual.wiki/Document/graphtheoryusermanual.346702481/view}},\n"
	"    note = {Accessed: 2020-01-13},\n"
	"}";

/// All bibliographic entries as an array.
static constexpr std::array<std::string_view, num_bib_entries> list_of_references = []()
{
	return std::array<std::string_view, num_bib_entries>{
		Alemany2024b,
		Alemany2024a,
		Alemany2023a,
		Alemany2022a,
		Alemany2022b,
		Alemany2022c,
		Anderson2021a,
		Macutek2021a,
		Alemany2020a,
		Alemany2020b,
		Johnson2020a,
		Alemany2019a,
		Anderson2019a,
		English2019a,
		Ferrer2019a,
		Nurse2019a,
		Ferrer2018a,
		Nurse2018a,
		Esteban2017a,
		Kahane2017a,
		Futrell2015a,
		Jing2015a,
		Ferrer2014a,
		Pitler2013a,
		Gomez2011a,
		Liu2010a,
		Gildea2007a,
		Ferrer2004a,
		Hochberg2003a,
		Cormen2001a,
		Iordanskii1987a,
		Wright1986a,
		Chung1984a,
		Colbourn1981a,
		Wilf1981a,
		Beyer1980a,
		Shiloach1979a,
		Nijenhuis1978a,
		Aho1974a,
		Harary1973a,
		Karp1972a,
		Harary1969a,
		Otter1948a,
		Pruefer1918a,
		OEIS_A000055,
		OEIS_A000081,
		OEIS_kLinearTrees,
		GMP_library,
		MathWorld_SpiderGraph,
		GiacXcas_Manual,
	};
}();

// clang-format on

#endif

} // namespace detail
} // namespace lal
