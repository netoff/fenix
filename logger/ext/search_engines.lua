require('url')

local SEARCH_ENGINES = {
	['1.cz'] = {'1.cz', 'q'},
  ['www.1.cz'] = {'1.cz', 'q'},
--123people
  ['www.123people.com'] = {'123people', 'search_term'},
  ['www.123people.de'] = {'123people', 'search_term'},
  ['www.123people.es'] = {'123people', 'search_term'},
  ['www.123people.fr'] = {'123people', 'search_term'},
-- 1und1
  ['portal.1und1.de'] = {'1und1', 'search'},
  ['search.1und1.de'] = {'1und1', 'su'},
-- 3271
  ['nmsearch.3721.com'] = {'3271', 'p'},
  ['seek.3721.com'] = {'3271', 'p'},
-- A9
  ['www.a9.com'] = {'A9', ''},
  ['a9.com'] = {'A9', ''},
-- Abacho
  ['search.abacho.com'] = {'Abacho', 'q'},
-- ABCsøk
  ['abcsok.no'] = {'ABCsøk', 'q'},
  ['www.abcsok.no'] = {'ABCsøk', 'q'},
-- about
  ['search.about.com'] = {'About', 'terms'},
-- Acoon
  ['www.acoon.de'] = {'Acoon', 'begriff'},
-- Acont
  ['acont.de'] = {'Acont', 'query'},
-- Alexa
  ['www.alexa.com'] = {'Alexa', 'q'},
  ['alexa.com'] = {'Alexa', 'q'},
-- Alice Adsl
  ['rechercher.aliceadsl.fr'] = {'Alice Adsl', 'qs'},
  ['search.alice.it'] = {'Alice (powered by Virgilio)', 'qt'},
  ['ricerca.alice.it'] = {'Alice (powered by Virgilio)', 'qs'},
-- Allesklar
  ['www.allesklar.de'] = {'Allesklar', 'words'},
-- AllTheWeb
  ['www.alltheweb.com'] = {'AllTheWeb', 'q'},
-- all.by
  ['all.by'] = {'All.by', 'query'},
-- Altavista
  ['www.altavista.com'] = {'AltaVista', 'q'},
  ['listings.altavista.com'] = {'AltaVista', 'q'},
  ['www.altavista.de'] = {'AltaVista', 'q'},
  ['altavista.fr'] = {'AltaVista', 'q'},
  ['de.altavista.com'] = {'AltaVista', 'q'},
  ['fr.altavista.com'] = {'AltaVista', 'q'},
  ['es.altavista.com'] = {'AltaVista', 'q'},
  ['www.altavista.fr'] = {'AltaVista', 'q'},
  ['search.altavista.com'] = {'AltaVista', 'q'},
  ['search.fr.altavista.com'] = {'AltaVista', 'q'},
  ['se.altavista.com'] = {'AltaVista', 'q'},
  ['be-nl.altavista.com'] = {'AltaVista', 'q'},
  ['be-fr.altavista.com'] = {'AltaVista', 'q'},
  ['it.altavista.com'] = {'AltaVista', 'q'},
  ['us.altavista.com'] = {'AltaVista', 'q'},
  ['nl.altavista.com'] = {'Altavista', 'q'},
  ['ch.altavista.com'] = {'AltaVista', 'q'},
-- Apollo Latvia
  ['apollo.lv/portal/search/'] = {'Apollo lv', 'q'},
-- APOLLO7
  ['www.apollo7.de'] = {'Apollo7', 'query'},
  ['apollo7.de'] = {'Apollo7', 'query'},
-- AOL
  ['search.aol.com'] = {'AOL', {'query', 'q'}},
  ['aolsearch.aol.com'] = {'AOL', {'query', 'q'}},
  ['www.aolrecherche.aol.fr'] = {'AOL', {'query', 'q'}},
  ['www.aolrecherches.aol.fr'] = {'AOL', {'query', 'q'}},
  ['www.aolimages.aol.fr'] = {'AOL', {'query', 'q'}},
  ['www.recherche.aol.fr'] = {'AOL', {'query', 'q'}},
  ['aolsearcht.aol.com'] = {'AOL', {'query', 'q'}},
  ['find.web.aol.com'] = {'AOL', {'query', 'q'}},
  ['recherche.aol.ca'] = {'AOL', {'query', 'q'}},
  ['aolsearch.aol.co.uk'] = {'AOL', {'query', 'q'}},
  ['search.aol.co.uk'] = {'AOL', {'query', 'q'}},
  ['aolrecherche.aol.fr'] = {'AOL', {'query', 'q'}},
  ['sucheaol.aol.de'] = {'AOL', {'query', 'q'}},
  ['suche.aol.de'] = {'AOL', {'query', 'q'}},
  ['suche.aolsvc.de'] = {'AOL', {'query', 'q'}},
  ['aolbusqueda.aol.com.mx'] = {'AOL', {'query', 'q'}},
  ['alicesuchet.aol.de'] = {'AOL', {'query', 'q'}},
  ['suche.aolsvc.de'] = {'AOL', {'query', 'q'}},
  ['suche.aol.de'] = {'AOL', {'query', 'q'}},
  ['alicesuche.aol.de'] = {'AOL', {'query', 'q'}},
  ['suchet2.aol.de'] = {'AOL', {'query', 'q'}},
  ['search.hp.my.aol.com.au'] = {'AOL', {'query', 'q'}},
  ['search.hp.my.aol.de'] = {'AOL', {'query', 'q'}},
-- Aport
  ['sm.aport.ru'] = {'Aport', 'r'},
-- Arcor
  ['www.arcor.de'] = {'Arcor', 'Keywords'},
-- Arianna (Libero.it)
  ['arianna.libero.it'] = {'Arianna', 'query'},
-- Ask
  ['www.ask.com'] = {'Ask', {'ask', 'q'}},
  ['web.ask.com'] = {'Ask', {'ask', 'q'}},
  ['ask.reference.com'] = {'Ask', 'q'},
  ['www.ask.co.uk'] = {'Ask', 'q'},
  ['uk.ask.com'] = {'Ask', 'q'},
  ['fr.ask.com'] = {'Ask', 'q'},
  ['de.ask.com'] = {'Ask', 'q'},
  ['es.ask.com'] = {'Ask', 'q'},
  ['it.ask.com'] = {'Ask', 'q'},
  ['nl.ask.com'] = {'Ask', 'q'},
  ['ask.jp'] = {'Ask', 'q'},
-- Atlas
  ['search.atlas.cz'] = {'Atlas', 'q'},
-- Austronaut
  ['www2.austronaut.at'] = {'Austronaut', 'begriff'},
-- Babylon
  ['search.babylon.com'] = {'Babylon (Powered by Google)', 'q'},
-- Baidu
  ['www.baidu.com'] = {'Baidu', {'wd', 'word', 'kw'}},
  ['www1.baidu.com'] = {'Baidu', {'wd', 'word', 'kw'}},
  ['zhidao.baidu.com'] = {'Baidu', {'wd', 'word', 'kw'}},
  ['tieba.baidu.com'] = {'Baidu', {'wd', 'word', 'kw'}},
  ['news.baidu.com'] = {'Baidu', {'wd', 'word', 'kw'}},
  ['web.gougou.com'] = {'Baidu', 'search'},
-- BBC
  ['search.bbc.co.uk'] = {'BBC', 'q'},
-- Bellnet
  ['www.suchmaschine.com'] = {'Bellnet', 'suchstr'},
-- Biglobe
  ['cgi.search.biglobe.ne.jp'] = {'Biglobe', 'q'},
-- Bild
  ['www.bild.t-online.de'] = {'Bild.de (enhanced by Google)', 'query'},
-- Bing
  ['www.bing.com'] = {'Bing', 'q'},
-- Blogdigger
  ['www.blogdigger.com'] = {'Blogdigger', 'q'},
-- Bloglines
  ['www.bloglines.com'] = {'Bloglines', 'q'},
-- Blogpulse
  ['www.blogpulse.com'] = {'Blogpulse', 'query'},
-- Bluewin
  ['search.bluewin.ch'] = {'Bluewin', 'query'},
-- Caloweb
  ['www.caloweb.de'] = {'Caloweb', 'q'},
-- Cegetel (Google)
  ['www.cegetel.net'] = {'Cegetel (Google)', 'q'},
-- Centrum
  ['search.centrum.cz'] = {'Centrum', 'q'},
  ['fulltext.centrum.cz'] = {'Centrum', 'q'},
  ['morfeo.centrum.cz'] = {'Centrum', 'q'},
-- Chello
  ['www.chello.fr'] = {'Chello', 'q1'},
-- Club Internet
  ['recherche.club-internet.fr'] = {'Club Internet', 'q'},
-- Clusty
  ['clusty.com'] = {'Clusty', 'query'},
-- Conduit
  ['search.conduit.com'] = {'Conduit.com', 'q'},
-- Comcast
  ['www.comcast.net'] = {'Comcast', 'query'},
  ['search.comcast.net'] = {'Comcast', 'q'},
-- Comet systems
  ['search.cometsystems.com'] = {'CometSystems', 'q'},
-- Compuserve
  ['suche.compuserve.de'] = {'Compuserve.de (Powered by Google)', 'q'},
  ['websearch.cs.com'] = {'Compuserve.com (Enhanced by Google)', 'query'},
-- Copernic
  ['metaresults.copernic.com'] = {'Copernic', ' '},
-- Crossbot
  ['www.crossbot.de'] = {'Crossbot', 'q'},
-- Cuil
  ['www.cuil.com'] = {'Cuil', 'q'},
-- Daemon search
  ['www.daemon-search.com'] = {'Daemon search', 'q'},
-- DasOertliche
  ['www.dasoertliche.de'] = {'DasOertliche', 'kw'},
-- DasTelefonbuch
  ['www.4call.dastelefonbuch.de'] = {'DasTelefonbuch', 'kw'},
-- Defind.de
  ['suche.defind.de'] = {'Defind.de', 'search'},
-- Delfi Latvia
  ['smart.delfi.lv'] = {'Delfi lv', 'q'},
-- Delfi
  ['otsing.delfi.ee'] = {'Delfi EE', 'q'},
-- Deskfeeds
  ['www.deskfeeds.com'] = {'Deskfeeds', 'sx'},
-- Dino
  ['www.dino-online.de'] = {'Dino', 'query'},
-- dir.com
  ['fr.dir.com'] = {'dir.com', 'req'},
-- dmoz
  ['dmoz.org'] = {'dmoz', 'search'},
  ['editors.dmoz.org'] = {'dmoz', 'search'},
  ['search.dmoz.org'] = {'dmoz', 'search'},
  ['www.dmoz.org'] = {'dmoz', 'search'},
-- Dogpile
  ['search.dogpile.com'] = {'Dogpile', 'q'},
  ['nbci.dogpile.com'] = {'Dogpile', 'q'},
-- earthlink
  ['search.earthlink.net'] = {'Earthlink', 'q'},
-- Eniro
  ['www.eniro.se'] = {'Eniro', 'q'},
-- Espotting
  ['affiliate.espotting.fr'] = {'Espotting', 'keyword'},
-- Eudip
  ['www.eudip.com'] = {'Eudip', ' '},
-- Eurip
  ['www.eurip.com'] = {'Eurip', 'q'},
-- Euroseek
  ['www.euroseek.com'] = {'Euroseek', 'string'},
-- Everyclick
  ['www.everyclick.com'] = {'Everyclick', 'keyword'},
-- Excite
  ['www.excite.it'] = {'Excite', 'q'},
  ['msxml.excite.com'] = {'Excite', 'qkw'},
  ['www.excite.fr'] = {'Excite', 'search'},
-- Exalead
  ['www.exalead.fr'] = {'Exalead', 'q'},
  ['www.exalead.com'] = {'Exalead', 'q'},
-- eo
  ['eo.st'] = {'eo', 'q'},
-- Feedminer
  ['www.feedminer.com'] = {'Feedminer', 'q'},
-- Feedster
  ['www.feedster.com'] = {'Feedster', ''},
-- Francite
  ['recherche.francite.com'] = {'Francite', 'name'},
  ['antisearch.francite.com'] = {'Francite', 'KEYWORDS'},
-- Fireball
  ['suche.fireball.de'] = {'Fireball', 'query'},
  ['www.fireball.de'] = {'Fireball', 'q'},
-- Firstfind
  ['www.firstsfind.com'] = {'Firstsfind', 'qry'},
-- Fixsuche
  ['www.fixsuche.de'] = {'Fixsuche', 'q'},
-- Flix
  ['www.flix.de'] = {'Flix.de', 'keyword'},
-- Forestle
  ['de.forestle.org'] = {'Forestle', 'q'},
  ['at.forestle.org'] = {'Forestle', 'q'},
  ['ch.forestle.org'] = {'Forestle', 'q'},
  ['us.forestle.org'] = {'Forestle', 'q'},
  ['fr.forestle.org'] = {'Forestle', 'q'},
-- Free
  ['search.free.fr'] = {'Free', 'q'},
  ['search1-2.free.fr'] = {'Free', 'q'},
  ['search1-1.free.fr'] = {'Free', 'q'},
-- Freecause
  ['search.freecause.com'] = {'FreeCause', 'q'},
-- Freenet
  ['suche.freenet.de'] = {'Freenet', 'query'},
-- FriendFeed
  ['friendfeed.com'] = {'FriendFeed', 'q'},
-- Froogle
  ['froogle.google.com'] = {'Google (Froogle)', 'q'},
  ['froogle.google.de'] = {'Google (Froogle)', 'q'},
  ['froogle.google.co.uk'] = {'Google (Froogle)', 'q'},
-- GAIS
  ['gais.cs.ccu.edu.tw'] = {'GAIS)', 'query'},
-- Gigablast
  ['www.gigablast.com'] = {'Gigablast', 'q'},
  ['blogs.gigablast.com'] = {'Gigablast (Blogs)', 'q'},
  ['travel.gigablast.com'] = {'Gigablast (Travel)', 'q'},
  ['dir.gigablast.com'] = {'Gigablast (Directory)', 'q'},
  ['gov.gigablast.com'] = {'Gigablast (Gov)', 'q'},
-- GMX
  ['suche.gmx.net'] = {'GMX', 'su'},
  ['www.gmx.net'] = {'GMX', 'su'},
-- Gnadenmeer
  ['www.gnadenmeer.de'] = {'Gnadenmeer', 'keyword'},
-- goo
  ['search.goo.ne.jp'] = {'goo', 'mt'},
  ['ocnsearch.goo.ne.jp'] = {'goo', 'mt'},
-- Google
  ['www.google.com'] = {'Google', 'q'},
  ['www2.google.com'] = {'Google', 'q'},
  ['ipv6.google.com'] = {'Google', 'q'},
  ['w.google.com'] = {'Google', 'q'},
  ['ww.google.com'] = {'Google', 'q'},
  ['wwwgoogle.com'] = {'Google', 'q'},
  ['www.goggle.com'] = {'Google', 'q'},
  ['www.gogole.com'] = {'Google', 'q'},
  ['www.gppgle.com'] = {'Google', 'q'},
  ['go.google.com'] = {'Google', 'q'},
  ['www.google.ae'] = {'Google', 'q'},
  ['www.google.as'] = {'Google', 'q'},
  ['www.google.at'] = {'Google', 'q'},
  ['wwwgoogle.at'] = {'Google', 'q'},
  ['ww.google.at'] = {'Google', 'q'},
  ['w.google.at'] = {'Google', 'q'},
  ['www.google.az'] = {'Google', 'q'},
  ['www.google.ba'] = {'Google', 'q'},
  ['www.google.be'] = {'Google', 'q'},
  ['www.google.bg'] = {'Google', 'q'},
  ['google.bg'] = {'Google', 'q'},
  ['www.google.bi'] = {'Google', 'q'},
  ['www.google.ca'] = {'Google', 'q'},
  ['ww.google.ca'] = {'Google', 'q'},
  ['w.google.ca'] = {'Google', 'q'},
  ['www.google.cc'] = {'Google', 'q'},
  ['www.google.cd'] = {'Google', 'q'},
  ['www.google.cg'] = {'Google', 'q'},
  ['www.google.ch'] = {'Google', 'q'},
  ['ww.google.ch'] = {'Google', 'q'},
  ['w.google.ch'] = {'Google', 'q'},
  ['www.google.ci'] = {'Google', 'q'},
  ['www.google.cl'] = {'Google', 'q'},
  ['www.google.cn'] = {'Google', 'q'},
  ['www.google.co'] = {'Google', 'q'},
  ['www.google.cz'] = {'Google', 'q'},
  ['wwwgoogle.cz'] = {'Google', 'q'},
  ['www.google.de'] = {'Google', 'q'},
  ['ww.google.de'] = {'Google', 'q'},
  ['w.google.de'] = {'Google', 'q'},
  ['wwwgoogle.de'] = {'Google', 'q'},
  ['www.google.ee'] = {'Google', 'q'},
  ['www.google.dj'] = {'Google', 'q'},
  ['www.google.dk'] = {'Google', 'q'},
  ['www.google.es'] = {'Google', 'q'},
  ['www.google.fi'] = {'Google', 'q'},
  ['www.google.fm'] = {'Google', 'q'},
  ['gogole.fr'] = {'Google', 'q'},
  ['www.gogole.fr'] = {'Google', 'q'},
  ['wwwgoogle.fr'] = {'Google', 'q'},
  ['ww.google.fr'] = {'Google', 'q'},
  ['w.google.fr'] = {'Google', 'q'},
  ['www.google.fr'] = {'Google', 'q'},
  ['www.google.fr.'] = {'Google', 'q'},
  ['google.fr'] = {'Google', 'q'},
  ['www.google.gg'] = {'Google', 'q'},
  ['google.ge'] = {'Google', 'q'},
  ['w.google.ge'] = {'Google', 'q'},
  ['ww.google.ge'] = {'Google', 'q'},
  ['www.google.ge'] = {'Google', 'q'},
  ['google.gr'] = {'Google', 'q'},
  ['www.googel.fi'] = {'Google', 'q'},
  ['www.google.gl'] = {'Google', 'q'},
  ['www.google.gm'] = {'Google', 'q'},
  ['www.google.gr'] = {'Google', 'q'},
  ['www.google.hn'] = {'Google', 'q'},
  ['www.google.hr'] = {'Google', 'q'},
  ['www.google.hu'] = {'Google', 'q'},
  ['www.google.ie'] = {'Google', 'q'},
  ['www.google.is'] = {'Google', 'q'},
  ['www.google.it'] = {'Google', 'q'},
  ['www.google.jo'] = {'Google', 'q'},
  ['www.google.kz'] = {'Google', 'q'},
  ['www.google.li'] = {'Google', 'q'},
  ['www.google.lk'] = {'Google', 'q'},
  ['www.google.lt'] = {'Google', 'q'},
  ['www.google.lu'] = {'Google', 'q'},
  ['www.google.lv'] = {'Google', 'q'},
  ['www.google.md'] = {'Google', 'q'},
  ['www.google.ms'] = {'Google', 'q'},
  ['www.google.mu'] = {'Google', 'q'},
  ['www.google.mw'] = {'Google', 'q'},
  ['www.google.nl'] = {'Google', 'q'},
  ['www.google.no'] = {'Google', 'q'},
  ['www.google.pl'] = {'Google', 'q'},
  ['www.google.pn'] = {'Google', 'q'},
  ['www.google.pt'] = {'Google', 'q'},
  ['www.google.ro'] = {'Google', 'q'},
  ['www.google.rs'] = {'Google', 'q'},
  ['www.google.ru'] = {'Google', 'q'},
  ['www.google.rw'] = {'Google', 'q'},
  ['www.google.se'] = {'Google', 'q'},
  ['www.google.sh'] = {'Google', 'q'},
  ['www.google.si'] = {'Google', 'q'},
  ['www.google.sk'] = {'Google', 'q'},
  ['www.google.sm'] = {'Google', 'q'},
  ['www.google.sn'] = {'Google', 'q'},
  ['www.google.td'] = {'Google', 'q'},
  ['www.google.tt'] = {'Google', 'q'},
  ['www.google.uz'] = {'Google', 'q'},
  ['www.google.vg'] = {'Google', 'q'},
  ['www.google.co.cr'] = {'Google', 'q'},
  ['www.google.co.gg'] = {'Google', 'q'},
  ['www.google.co.hu'] = {'Google', 'q'},
  ['www.google.co.id'] = {'Google', 'q'},
  ['www.google.co.il'] = {'Google', 'q'},
  ['www.google.co.in'] = {'Google', 'q'},
  ['www.google.co.je'] = {'Google', 'q'},
  ['www.google.co.jp'] = {'Google', 'q'},
  ['www.google.co.ls'] = {'Google', 'q'},
  ['www.google.co.ke'] = {'Google', 'q'},
  ['www.google.co.kr'] = {'Google', 'q'},
  ['www.google.co.ma'] = {'Google', 'q'},
  ['www.google.co.nz'] = {'Google', 'q'},
  ['www.google.co.th'] = {'Google', 'q'},
  ['www.google.co.uk'] = {'Google', 'q'},
  ['www.google.co.ve'] = {'Google', 'q'},
  ['www.google.co.za'] = {'Google', 'q'},
  ['www.google.co.zw'] = {'Google', 'q'},
  ['www.google.com.ar'] = {'Google', 'q'},
  ['www.google.com.au'] = {'Google', 'q'},
  ['www.google.com.bh'] = {'Google', 'q'},
  ['www.google.com.bo'] = {'Google', 'q'},
  ['www.google.com.br'] = {'Google', 'q'},
  ['www.google.com.by'] = {'Google', 'q'},
  ['www.google.com.co'] = {'Google', 'q'},
  ['www.google.com.cu'] = {'Google', 'q'},
  ['www.google.com.do'] = {'Google', 'q'},
  ['www.google.com.ec'] = {'Google', 'q'},
  ['www.google.com.eg'] = {'Google', 'q'},
  ['www.google.com.fj'] = {'Google', 'q'},
  ['www.google.com.gh'] = {'Google', 'q'},
  ['www.google.com.gi'] = {'Google', 'q'},
  ['www.google.com.gr'] = {'Google', 'q'},
  ['www.google.com.gt'] = {'Google', 'q'},
  ['www.google.com.hk'] = {'Google', 'q'},
  ['www.google.com.jm'] = {'Google', 'q'},
  ['www.google.com.kw'] = {'Google', 'q'},
  ['www.google.com.ly'] = {'Google', 'q'},
  ['www.google.com.mt'] = {'Google', 'q'},
  ['www.google.com.mx'] = {'Google', 'q'},
  ['www.google.com.my'] = {'Google', 'q'},
  ['www.google.com.na'] = {'Google', 'q'},
  ['www.google.com.nf'] = {'Google', 'q'},
  ['www.google.com.ng'] = {'Google', 'q'},
  ['www.google.com.ni'] = {'Google', 'q'},
  ['www.google.com.np'] = {'Google', 'q'},
  ['www.google.com.om'] = {'Google', 'q'},
  ['www.google.com.pa'] = {'Google', 'q'},
  ['www.google.com.pe'] = {'Google', 'q'},
  ['www.google.com.ph'] = {'Google', 'q'},
  ['www.google.com.pk'] = {'Google', 'q'},
  ['www.google.com.pl'] = {'Google', 'q'},
  ['www.google.com.pr'] = {'Google', 'q'},
  ['www.google.com.py'] = {'Google', 'q'},
  ['www.google.com.qa'] = {'Google', 'q'},
  ['www.google.com.ru'] = {'Google', 'q'},
  ['www.google.com.sa'] = {'Google', 'q'},
  ['www.google.com.sg'] = {'Google', 'q'},
  ['www.google.com.sv'] = {'Google', 'q'},
  ['www.google.com.tr'] = {'Google', 'q'},
  ['www.google.com.tw'] = {'Google', 'q'},
  ['www.google.com.ua'] = {'Google', 'q'},
  ['www.google.com.uy'] = {'Google', 'q'},
  ['www.google.com.vc'] = {'Google', 'q'},
  ['www.google.com.vn'] = {'Google', 'q'},
-- Powered by Google
  ['www.charter.net'] = {'Google', 'q'},
  ['brisbane.t-online.de'] = {'Google', 'q'},
  ['miportal.bellsouth.net'] = {'Google', 'string'},
  ['home.bellsouth.net'] = {'Google', 'string'},
  ['pesquisa.clix.pt'] = {'Google', 'q'},
  ['google.startsiden.no'] = {'Google', 'q'},
  ['google.startpagina.nl'] = {'Google', 'q'},
  ['search.peoplepc.com'] = {'Google', 'q'},
  ['www.google.interia.pl'] = {'Google', 'q'},
  ['buscador.terra.es'] = {'Google', 'query'},
  ['buscador.terra.cl'] = {'Google', 'query'},
  ['buscador.terra.com.br'] = {'Google', 'query'},
  ['www.adelphia.net'] = {'Google', 'q'},
  ['so.qq.com'] = {'Google', 'word'},
  ['misc.skynet.be'] = {'Google', 'keywords'},
  ['verden.abcsok.no'] = {'Google', 'q'},
  ['search3.incredimail.com'] = {'Google', 'q'},
  ['search.incredimail.com'] = {'Google', 'q'},
  ['search.sweetim.com'] = {'Google', 'q'},
-- Google Earth
  ['www.googleearth.de'] = {'Google', 'q'},
  ['www.googleearth.fr'] = {'Google', 'q'},
-- Google Blogsearch
  ['blogsearch.google.com'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.net'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.at'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.be'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.ch'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.de'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.es'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.fr'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.it'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.nl'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.pl'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.ru'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.co.in'] = {'Google Blogsearch', 'q'},
  ['blogsearch.google.co.uk'] = {'Google Blogsearch', 'q'},
-- Google translation
  ['translate.google.com'] = {'Google Translations', 'q'},
-- Google Images
  ['images.google.com'] = {'Google Images', 'q'},
  ['images.google.at'] = {'Google Images', 'q'},
  ['images.google.be'] = {'Google Images', 'q'},
  ['images.google.bg'] = {'Google Images', 'q'},
  ['images.google.ca'] = {'Google Images', 'q'},
  ['images.google.ch'] = {'Google Images', 'q'},
  ['images.google.ci'] = {'Google Images', 'q'},
  ['images.google.cz'] = {'Google Images', 'q'},
  ['images.google.de'] = {'Google Images', 'q'},
  ['images.google.dk'] = {'Google Images', 'q'},
  ['images.google.ee'] = {'Google Images', 'q'},
  ['images.google.es'] = {'Google Images', 'q'},
  ['images.google.fi'] = {'Google Images', 'q'},
  ['images.google.fr'] = {'Google Images', 'q'},
  ['images.google.gg'] = {'Google Images', 'q'},
  ['images.google.gr'] = {'Google Images', 'q'},
  ['images.google.hr'] = {'Google Images', 'q'},
  ['images.google.hu'] = {'Google Images', 'q'},
  ['images.google.it'] = {'Google Images', 'q'},
  ['images.google.lt'] = {'Google Images', 'q'},
  ['images.google.ms'] = {'Google Images', 'q'},
  ['images.google.nl'] = {'Google Images', 'q'},
  ['images.google.no'] = {'Google Images', 'q'},
  ['images.google.pl'] = {'Google Images', 'q'},
  ['images.google.pt'] = {'Google Images', 'q'},
  ['images.google.ro'] = {'Google Images', 'q'},
  ['images.google.ru'] = {'Google Images', 'q'},
  ['images.google.se'] = {'Google Images', 'q'},
  ['images.google.sk'] = {'Google Images', 'q'},
  ['images.google.co.id'] = {'Google Images', 'q'},
  ['images.google.co.il'] = {'Google Images', 'q'},
  ['images.google.co.in'] = {'Google Images', 'q'},
  ['images.google.co.jp'] = {'Google Images', 'q'},
  ['images.google.co.hu'] = {'Google Images', 'q'},
  ['images.google.co.kr'] = {'Google Images', 'q'},
  ['images.google.co.nz'] = {'Google Images', 'q'},
  ['images.google.co.th'] = {'Google Images', 'q'},
  ['images.google.co.tw'] = {'Google Images', 'q'},
  ['images.google.co.uk'] = {'Google Images', 'q'},
  ['images.google.co.ve'] = {'Google Images', 'q'},
  ['images.google.co.za'] = {'Google Images', 'q'},
  ['images.google.com.ar'] = {'Google Images', 'q'},
  ['images.google.com.au'] = {'Google Images', 'q'},
  ['images.google.com.br'] = {'Google Images', 'q'},
  ['images.google.com.cu'] = {'Google Images', 'q'},
  ['images.google.com.do'] = {'Google Images', 'q'},
  ['images.google.com.gr'] = {'Google Images', 'q'},
  ['images.google.com.hk'] = {'Google Images', 'q'},
  ['images.google.com.kw'] = {'Google Images', 'q'},
  ['images.google.com.mx'] = {'Google Images', 'q'},
  ['images.google.com.my'] = {'Google Images', 'q'},
  ['images.google.com.pe'] = {'Google Images', 'q'},
  ['images.google.com.sa'] = {'Google Images', 'q'},
  ['images.google.com.tr'] = {'Google Images', 'q'},
  ['images.google.com.tw'] = {'Google Images', 'q'},
  ['images.google.com.ua'] = {'Google Images', 'q'},
  ['images.google.com.vn'] = {'Google Images', 'q'},
-- Google News
  ['news.google.com'] = {'Google News', 'q'},
  ['news.google.at'] = {'Google News', 'q'},
  ['news.google.ca'] = {'Google News', 'q'},
  ['news.google.ch'] = {'Google News', 'q'},
  ['news.google.cl'] = {'Google News', 'q'},
  ['news.google.de'] = {'Google News', 'q'},
  ['news.google.es'] = {'Google News', 'q'},
  ['news.google.fr'] = {'Google News', 'q'},
  ['news.google.ie'] = {'Google News', 'q'},
  ['news.google.it'] = {'Google News', 'q'},
  ['news.google.lt'] = {'Google News', 'q'},
  ['news.google.lu'] = {'Google News', 'q'},
  ['news.google.se'] = {'Google News', 'q'},
  ['news.google.sm'] = {'Google News', 'q'},
  ['news.google.co.in'] = {'Google News', 'q'},
  ['news.google.co.jp'] = {'Google News', 'q'},
  ['news.google.co.uk'] = {'Google News', 'q'},
  ['news.google.co.ve'] = {'Google News', 'q'},
  ['news.google.com.ar'] = {'Google News', 'q'},
  ['news.google.com.au'] = {'Google News', 'q'},
  ['news.google.com.co'] = {'Google News', 'q'},
  ['news.google.com.hk'] = {'Google News', 'q'},
  ['news.google.com.ly'] = {'Google News', 'q'},
  ['news.google.com.mx'] = {'Google News', 'q'},
  ['news.google.com.pe'] = {'Google News', 'q'},
  ['news.google.com.tw'] = {'Google News', 'q'},
-- Google syndicated search
  ['googlesyndicatedsearch.com'] = {'Google syndicated search', 'q'},
-- Goyellow.de
  ['www.goyellow.de'] = {'GoYellow.de', 'MDN'},
-- Gule Sider
  ['www.gulesider.no'] = {'Gule Sider', 'q'},
-- HighBeam
  ['www.highbeam.com'] = {'HighBeam', 'Q'},
-- Hit-Parade
  ['recherche.hit-parade.com'] = {'Hit-Parade', 'p7'},
  ['class.hit-parade.com'] = {'Hit-Parade', 'p7'},
-- Holmes.ge
  ['holmes.ge'] = {'Holmes', 'q'},
-- Hooseek.com
  ['www.hooseek.com'] = {'Hooseek', 'recherche'},
-- Hotbot via Lycos
  ['hotbot.lycos.com'] = {'Hotbot (Lycos)', 'query'},
  ['search.hotbot.de'] = {'Hotbot', 'query'},
  ['search.hotbot.fr'] = {'Hotbot', 'query'},
  ['www.hotbot.com'] = {'Hotbot', 'query'},
-- 1stekeuze
  ['zoek.1stekeuze.nl'] = {'1stekeuze', 'terms'},
-- Infoseek
  ['search.www.infoseek.co.jp'] = {'Infoseek', 'qt'},
-- Icerocket
  ['blogs.icerocket.com'] = {'Icerocket', 'qt'},
-- ICQ
  ['www.icq.com'] = {'ICQ', 'q'},
  ['search.icq.com'] = {'ICQ', 'q'},
-- Ilse
  ['spsearch.ilse.nl'] = {'Startpagina', 'search_for'},
  ['be.ilse.nl'] = {'Ilse BE', 'query'},
  ['search.ilse.nl'] = {'Ilse NL', 'search_for'},
-- Iwon
  ['search.iwon.com'] = {'Iwon', 'searchfor'},
-- Ixquick
  ['ixquick.com'] = {'Ixquick', 'query'},
  ['www.eu.ixquick.com'] = {'Ixquick', 'query'},
  ['us.ixquick.com'] = {'Ixquick', 'query'},
  ['s1.us.ixquick.com'] = {'Ixquick', 'query'},
  ['s2.us.ixquick.com'] = {'Ixquick', 'query'},
  ['s3.us.ixquick.com'] = {'Ixquick', 'query'},
  ['s4.us.ixquick.com'] = {'Ixquick', 'query'},
  ['s5.us.ixquick.com'] = {'Ixquick', 'query'},
  ['eu.ixquick.com'] = {'Ixquick', 'query'},
-- Jyxo
  ['jyxo.cz'] = {'Jyxo', 'q'},
-- Jungle Spider
  ['www.jungle-spider.de'] = {'Jungle Spider', 'qry'},
-- Kartoo
  ['kartoo.com'] = {'Kartoo', ''},
  ['kartoo.de'] = {'Kartoo', ''},
  ['kartoo.fr'] = {'Kartoo', ''},
-- Kataweb
  ['www.kataweb.it'] = {'Kataweb', 'q'},
-- Klug suchen
  ['www.klug-suchen.de'] = {'Klug suchen!', 'query'},
-- kostenlos
  ['www.kostenlos.de'] = {'kostenlos.de', 'q'},
-- Kvasir
  ['kvasir.no'] = {'Kvasir', 'q'},
  ['www.kvasir.no'] = {'Kvasir', 'q'},
-- Latne
  ['www.latne.lv'] = {'Latne', 'q'},
-- La Toile Du Québec via Google
  ['google.canoe.com'] = {'La Toile Du Québec (Google)', 'q'},
  ['www.toile.com'] = {'La Toile Du Québec (Google)', 'q'},
  ['web.toile.com'] = {'La Toile Du Québec (Google)', 'q'},
-- La Toile Du Québec
  ['recherche.toile.qc.ca'] = {'La Toile Du Québec', 'query'},
-- Live.com
  ['search.live.com'] = {'Live', 'q'},
  ['beta.search.live.com'] = {'Live', 'q'},
  ['www.live.com'] = {'Live', 'q'},
  ['search.msn.com'] = {'Live', 'q'},
  ['beta.search.msn.fr'] = {'Live', 'q'},
  ['search.msn.fr'] = {'Live', 'q'},
  ['search.msn.es'] = {'Live', 'q'},
  ['search.msn.se'] = {'Live', 'q'},
  ['search.latam.msn.com'] = {'Live', 'q'},
  ['search.msn.nl'] = {'Live', 'q'},
  ['leguide.fr.msn.com'] = {'Live', 's'},
  ['leguide.msn.fr'] = {'Live', 's'},
  ['search.msn.co.jp'] = {'Live', 'q'},
  ['search.msn.no'] = {'Live', 'q'},
  ['search.msn.at'] = {'Live', 'q'},
  ['search.msn.com.hk'] = {'Live', 'q'},
  ['search.t1msn.com.mx'] = {'Live', 'q'},
  ['fr.ca.search.msn.com'] = {'Live', 'q'},
  ['search.msn.be'] = {'Live', 'q'},
  ['search.fr.msn.be'] = {'Live', 'q'},
  ['search.msn.it'] = {'Live', 'q'},
  ['sea.search.msn.it'] = {'Live', 'q'},
  ['sea.search.msn.fr'] = {'Live', 'q'},
  ['sea.search.msn.de'] = {'Live', 'q'},
  ['sea.search.msn.com'] = {'Live', 'q'},
  ['sea.search.fr.msn.be'] = {'Live', 'q'},
  ['search.msn.com.tw'] = {'Live', 'q'},
  ['search.msn.de'] = {'Live', 'q'},
  ['search.msn.co.uk'] = {'Live', 'q'},
  ['search.msn.co.za'] = {'Live', 'q'},
  ['search.msn.ch'] = {'Live', 'q'},
  ['search.msn.es'] = {'Live', 'q'},
  ['search.msn.com.br'] = {'Live', 'q'},
  ['search.ninemsn.com.au'] = {'Live', 'q'},
  ['search.msn.dk'] = {'Live', 'q'},
  ['search.arabia.msn.com'] = {'Live', 'q'},
  ['search.prodigy.msn.com'] = {'Live', 'q'},
-- Looksmart
  ['www.looksmart.com'] = {'Looksmart', 'key'},
-- Lo.st
  ['lo.st'] = {'Lo.st (Powered by Google)', 'x_query'},
-- Lycos
  ['search.lycos.com'] = {'Lycos', 'query'},
  ['vachercher.lycos.fr'] = {'Lycos', 'query'},
  ['www.lycos.fr'] = {'Lycos', 'query'},
  ['suche.lycos.de'] = {'Lycos', 'query'},
  ['search.lycos.de'] = {'Lycos', 'query'},
  ['sidesearch.lycos.com'] = {'Lycos', 'query'},
  ['www.multimania.lycos.fr'] = {'Lycos', 'query'},
  ['buscador.lycos.es'] = {'Lycos', 'query'},
-- maailm.com
  ['www.maailm.com'] = {'maailm.com', 'tekst'},
-- Mail.ru
  ['go.mail.ru'] = {'Mailru', 'q'},
-- Mamma
  ['mamma.com'] = {'Mamma', 'query'},
  ['mamma75.mamma.com'] = {'Mamma', 'query'},
  ['www.mamma.com'] = {'Mamma', 'query'},
-- Meceoo
  ['www.meceoo.fr'] = {'Meceoo', 'kw'},
-- Mediaset
  ['servizi.mediaset.it'] = {'Mediaset', 'searchword'},
-- Meta
  ['meta.ua'] = {'Meta.ua', 'q'},
-- Metacrawler
  ['search.metacrawler.com'] = {'Metacrawler', 'general'},
-- Metager
  ['meta.rrzn.uni-hannover.de'] = {'Metager', 'eingabe'},
  ['www.metager.de'] = {'Metager', 'eingabe'},
-- Metager2
  ['www.metager2.de'] = {'Metager2', 'q'},
  ['metager2.de'] = {'Metager2', 'q'},
-- Meinestadt
  ['www.meinestadt.de'] = {'Meinestadt.de', 'words'},
  ['home.meinestadt.de'] = {'Meinestadt.de', 'words'},
-- Mister Wong
  ['www.mister-wong.com'] = {'Mister Wong', 'keywords'},
  ['www.mister-wong.de'] = {'Mister Wong', 'keywords'},
-- Monstercrawler
  ['www.monstercrawler.com'] = {'Monstercrawler', 'qry'},
-- Mozbot
  ['www.mozbot.fr'] = {'mozbot', 'q'},
  ['www.mozbot.co.uk'] = {'mozbot', 'q'},
  ['www.mozbot.com'] = {'mozbot', 'q'},
-- El Mundo
  ['ariadna.elmundo.es'] = {'El Mundo', 'q'},
-- MySpace
  ['searchservice.myspace.com'] = {'MySpace', 'qry'},
-- MyWebSearch
  ['kf.mysearch.myway.com'] = {'MyWebSearch', 'searchfor'},
  ['ms114.mysearch.com'] = {'MyWebSearch', 'searchfor'},
  ['ms146.mysearch.com'] = {'MyWebSearch', 'searchfor'},
  ['mysearch.myway.com'] = {'MyWebSearch', 'searchfor'},
  ['searchfr.myway.com'] = {'MyWebSearch', 'searchfor'},
  ['ki.mysearch.myway.com'] = {'MyWebSearch', 'searchfor'},
  ['search.mywebsearch.com'] = {'MyWebSearch', 'searchfor'},
  ['www.mywebsearch.com'] = {'MyWebSearch', 'searchfor'},
-- Najdi
  ['www.najdi.si'] = {'Najdi.si', 'q'},
-- Naver
  ['search.naver.com'] = {'Naver', 'query'},
-- Needtofind
  ['ko.search.need2find.com'] = {'Needtofind', 'searchfor'},
-- Neti
  ['www.neti.ee'] = {'Neti', 'query'},
-- Netster
  ['www.netster.com'] = {'Netster', 'keywords'},
-- Netscape
  ['search-intl.netscape.com'] = {'Netscape', 'search'},
  ['www.netscape.fr'] = {'Netscape', 'q'},
  ['suche.netscape.de'] = {'Netscape', 'q'},
  ['search.netscape.com'] = {'Netscape', 'query'},
-- Nifty
  ['search.nifty.com'] = {'Nifty', 'q'},
-- Nigma
  ['www.nigma.ru'] = {'Nigma', 's'},
  ['nigma.ru'] = {'Nigma', 's'},
-- Nomade
  ['ie4.nomade.fr'] = {'Nomade', 's'},
  ['rechercher.nomade.aliceadsl.fr'] = {'Nomade (AliceADSL)', 's'},
  ['rechercher.nomade.fr'] = {'Nomade', 's'},
-- Northern Light
  ['www.northernlight.com'] = {'Northern Light', 'qr'},
-- Numéricable
  ['www.numericable.fr'] = {'Numéricable', 'query'},
-- Onet
  ['szukaj.onet.pl'] = {'Onet.pl', 'qt'},
-- Online.no
  ['www.online.no'] = {'Online.no', 'q'},
  ['online.no'] = {'Online.no', 'q'},
-- Opera
  ['search.opera.com'] = {'Opera', 'search'},
-- Openfind
  ['wps.openfind.com.tw'] = {'Openfind (Websearch)', 'query'},
  ['bbs2.openfind.com.tw'] = {'Openfind (BBS)', 'query'},
  ['news.openfind.com.tw'] = {'Openfind (News)', 'query'},
-- Opplysningen 1881
  ['www.1881.no'] = {'Opplysningen 1881', 'Query'},
-- Overture
  ['www.overture.com'] = {'Overture', 'Keywords'},
  ['www.fr.overture.com'] = {'Overture', 'Keywords'},
-- Paperball
  ['suche.paperball.de'] = {'Paperball', 'query'},
-- Picsearch
  ['www.picsearch.com'] = {'Picsearch', 'q'},
-- Plazoo
  ['www.plazoo.com'] = {'Plazoo', 'q'},
-- Poisk.Ru
  ['poisk.ru'] = {'Poisk.Ru', 'text'},
-- Postami
  ['www.postami.com'] = {'Postami', 'query'},
-- qip
  ['start.qip.ru'] = {'qip.ru', 'query'},
  ['search.qip.ru'] = {'qip.ru', 'query'},
-- Quick searches
  ['data.quicksearches.net'] = {'QuickSearches', 'q'},
-- Qualigo
  ['www.qualigo.at'] = {'Qualigo', 'q'},
  ['www.qualigo.ch'] = {'Qualigo', 'q'},
  ['www.qualigo.de'] = {'Qualigo', 'q'},
  ['www.qualigo.nl'] = {'Qualigo', 'q'},
-- Rambler
  ['nova.rambler.ru'] = {'Rambler', {'query', 'words'}},
  ['search.rambler.ru'] = {'Rambler', 'words'},
  ['www.rambler.ru'] = {'Rambler', 'words'},
-- Reacteur.com
  ['www.reacteur.com'] = {'Reacteur', 'kw'},
-- Sapo
  ['pesquisa.sapo.pt'] = {'Sapo', 'q'},
-- Search.com
  ['www.search.com'] = {'Search.com', 'q'},
-- Search.ch
  ['www.search.ch'] = {'Search.ch', 'q'},
-- Search a lot
  ['www.searchalot.com'] = {'Searchalot', 'query'},
-- Seek
  ['www.seek.fr'] = {'Searchalot', 'qry_str'},
-- Seekport
  ['www.seekport.at'] = {'Seekport', 'query'},
  ['www.seekport.de'] = {'Seekport', 'query'},
  ['www.seekport.es'] = {'Seekport', 'query'},
  ['www.seekport.fr'] = {'Seekport', 'query'},
  ['www.seekport.it'] = {'Seekport', 'query'},
  ['www.seekport.co.uk'] = {'Seekport', 'query'},
-- Seekport (blogs)
  ['blogs.seekport.at'] = {'Seekport (Blogs)', 'query'},
  ['blogs.seekport.de'] = {'Seekport (Blogs)', 'query'},
  ['blogs.seekport.es'] = {'Seekport (Blogs)', 'query'},
  ['blogs.seekport.fr'] = {'Seekport (Blogs)', 'query'},
  ['blogs.seekport.it'] = {'Seekport (Blogs)', 'query'},
  ['blogs.seekport.co.uk'] = {'Seekport (Blogs)', 'query'},
-- Seekport (news)
  ['news.seekport.at'] = {'Seekport (News)', 'query'},
  ['news.seekport.de'] = {'Seekport (News)', 'query'},
  ['news.seekport.es'] = {'Seekport (News)', 'query'},
  ['news.seekport.fr'] = {'Seekport (News)', 'query'},
  ['news.seekport.it'] = {'Seekport (News)', 'query'},
  ['news.seekport.co.uk'] = {'Seekport (News)', 'query'},
-- Searchscout
  ['www.searchscout.com'] = {'Search Scout', 'gt_keywords'},
-- Searchy
  ['www.searchy.co.uk'] = {'Searchy', 'search_term'},
-- Sesam
  ['sesam.no'] = {'Sesam', 'q'},
-- Setooz
  ['bg.setooz.com'] = {'Setooz', 'query'},
  ['el.setooz.com'] = {'Setooz', 'query'},
  ['et.setooz.com'] = {'Setooz', 'query'},
  ['fi.setooz.com'] = {'Setooz', 'query'},
  ['hu.setooz.com'] = {'Setooz', 'query'},
  ['lt.setooz.com'] = {'Setooz', 'query'},
  ['lv.setooz.com'] = {'Setooz', 'query'},
  ['no.setooz.com'] = {'Setooz', 'query'},
  ['pl.setooz.com'] = {'Setooz', 'query'},
  ['sk.setooz.com'] = {'Setooz', 'query'},
  ['sv.setooz.com'] = {'Setooz', 'query'},
  ['tr.setooz.com'] = {'Setooz', 'query'},
  ['uk.setooz.com'] = {'Setooz', 'query'},
-- Seznam
  ['search.seznam.cz'] = {'Seznam', 'q'},
  ['search1.seznam.cz'] = {'Seznam', 'q'},
  ['search2.seznam.cz'] = {'Seznam', 'q'},
-- Sharelook
  ['www.sharelook.fr'] = {'Sharelook', 'keyword'},
  ['www.sharelook.de'] = {'Sharelook', 'keyword'},
-- Skynet
  ['search.skynet.be'] = {'Skynet', 'keywords'},
-- soso.com
  ['www.soso.com'] = {'Soso', 'w'},
-- Sphere
  ['www.sphere.com'] = {'Sphere', 'q'},
-- Start.no
  ['www.start.no'] = {'Google', 'q'},
-- Startpagina
  ['startgoogle.startpagina.nl'] = {'Startpagina (Google)', 'q'},
-- Suchmaschine.com
  ['www.suchmaschine.com'] = {'Suchmaschine.com', 'suchstr'},
-- Suchnase
  ['www.suchnase.de'] = {'Suchnase', 'qkw'},
-- Supereva
  ['search.supereva.com'] = {'Supereva', 'q'},
-- Sympatico
  ['search.sympatico.msn.ca'] = {'Sympatico', 'q'},
  ['search.sli.sympatico.ca'] = {'Sympatico', 'q'},
  ['search.fr.sympatico.msn.ca'] = {'Sympatico', 'q'},
  ['sea.search.fr.sympatico.msn.ca'] = {'Sympatico', 'q'},
-- Technorati
  ['www.technorati.com'] = {'Technorati', ' '},
-- Teoma
  ['www.teoma.com'] = {'Teoma', 't'},
-- Tiscali
  ['rechercher.nomade.tiscali.fr'] = {'Tiscali', 's'},
  ['search-dyn.tiscali.it'] = {'Tiscali', 'key'},
  ['www.tiscali.co.uk'] = {'Tiscali', 'query'},
  ['search-dyn.tiscali.de'] = {'Tiscali', 'key'},
  ['hledani.tiscali.cz'] = {'Tiscali', 'query'},
-- Tixuma
  ['www.tixuma.de'] = {'Tixuma', 'sc'},
-- T-Online
  ['suche.t-online.de'] = {'T-Online', 'q'},
  ['navigationshilfe.t-online.de'] = {'T-Online', 'q'},
-- Trouvez.com
  ['www.trouvez.com'] = {'Trouvez.com', 'query'},
-- Trusted-Search
  ['www.trusted--search.com'] = {'Trusted Search', 'w'},
-- Twingly
  ['www.twingly.com'] = {'Twingly', 'q'},
-- Vinden
  ['zoek.vinden.nl'] = {'Vinden', 'query'},
-- Vindex
  ['www.vindex.nl'] = {'Vindex', 'search_for'},
-- Virgilio
  ['search.virgilio.it'] = {'Virgilio', 'qs'},
  ['ricerca.virgilio.it'] = {'Virgilio', 'qs'},
-- vivisimo
  ['vivisimo.com'] = {'Vivisimo', 'query'},
  ['search.vivisimo.com'] = {'Vivisimo', 'query'},
	
  ['de.vivisimo.com'] = {'Vivisimo', 'query'},
-- Voila
  ['search.voila.com'] = {'Voila', 'kw'},
  ['search.ke.voila.fr'] = {'Voila', 'rdata'},
  ['moteur.voila.fr'] = {'Voila', 'kw'},
  ['search.voila.fr'] = {'Voila', 'kw'},
  ['beta.voila.fr'] = {'Voila', 'kw'},
-- Volny
  ['volny.zlatestranky.cz'] = {'Volny', 'search'},
  ['web.volny.cz'] = {'Volny', 'search'},
-- Wanadoo
  ['search.ke.wanadoo.fr'] = {'Wanadoo', 'kw'},
  ['busca.wanadoo.es'] = {'Wanadoo', 'buscar'},
-- Web.de
  ['suche.web.de'] = {'Web.de (Websuche)', 'su'},
  ['dir.web.de'] = {'Web.de (Directory)', 'su'},
-- Webtip
  ['www.webtip.de'] = {'Webtip', 'keyword'},
-- X-recherche
  ['www.x-recherche.com'] = {'X-Recherche', 'mots'},
-- Yahoo
  ['search.yahoo.com'] = {'Yahoo!', 'p', 'search?p={k}'},
  ['ink.yahoo.com'] = {'Yahoo!', 'p'},
  ['ink.yahoo.fr'] = {'Yahoo!', 'p'},
  ['fr.ink.yahoo.com'] = {'Yahoo!', 'p'},
  ['search.yahoo.co.jp'] = {'Yahoo!', 'p'},
  ['search.yahoo.fr'] = {'Yahoo!', 'p'},
  ['fi.yahoo.com'] = {'Yahoo!', 'p'},
  ['ar.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['au.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['br.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['ch.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['ca.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['cade.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['cf.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['de.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['es.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['espanol.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['fr.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['id.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['it.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['kr.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['mx.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['nl.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['qc.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['ru.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['se.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['tw.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['uk.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['us.search.yahoo.com'] = {'Yahoo!', 'p'},
  ['www.yahoo.com.cn'] = {'Yahoo!', 'p'},
	
  ['au.yhs.search.yahoo.com'] = {'Yahoo!', 'p', 'avg/search?p={k}'},
  ['de.yhs.search.yahoo.com'] = {'Yahoo!', 'p', 'avg/search?p={k}'},
	
  ['de.dir.yahoo.com'] = {'Yahoo! Webverzeichnis', ''},
  ['cf.dir.yahoo.com'] = {'Yahoo! Directory', ''},
  ['fr.dir.yahoo.com'] = {'Yahoo! Directory', ''},
-- Yahoo! Images
  ['images.search.yahoo.com'] = {'Yahoo! Images', 'p'},
-- Yandex
  ['yandex.ru'] = {'Yandex', 'text', 'yandsearch?text={k}'},
  ['www.yandex.ru'] = {'Yandex', 'text'},
  ['search.yaca.yandex.ru'] = {'Yandex', 'text'},
  ['ya.ru'] = {'Yandex', 'text'},
  ['www.ya.ru'] = {'Yandex', 'text'},
-- Yandex Images
  ['images.yandex.ru'] = {'Yandex Images', 'text'},
-- Yasni
  ['www.yasni.de'] = {'Yasni', 'name'},
  ['www.yasni.com'] = {'Yasni', 'name'},
  ['www.yasni.co.uk'] = {'Yasni', 'name'},
  ['www.yasni.ch'] = {'Yasni', 'name'},
  ['www.yasni.at'] = {'Yasni', 'name'},
-- Yellowmap
  ['www.yellowmap.de'] = {'Yellowmap', ' '},
  ['yellowmap.de'] = {'Yellowmap', ' '},
-- Wanadoo
  ['search.ke.wanadoo.fr'] = {'Wanadoo', 'kw'},
  ['busca.wanadoo.es'] = {'Wanadoo', 'buscar'},
-- Wedoo
  ['fr.wedoo.com'] = {'Wedoo', 'keyword'},
-- Web.nl
  ['www.web.nl'] = {'Web.nl', 'query'},
-- Weborama
  ['www.weborama.fr'] = {'weborama', 'query'},
-- WebSearch
  ['is1.websearch.com'] = {'WebSearch', 'qkw'},
  ['www.websearch.com'] = {'WebSearch', 'qkw'},
  ['websearch.cs.com'] = {'WebSearch', 'query'},
-- Witch
  ['www.witch.de'] = {'Witch', 'search'},
-- WWW
  ['search.www.ee'] = {'www värav', 'query'},
-- WXS
  ['wxsl.nl'] = {'Planet Internet', 'q'},
-- Zoek
  ['www3.zoek.nl'] = {'Zoek', 'q'},
-- Zhongsou
  ['p.zhongsou.com'] = {'Zhongsou', 'w'},
-- Zoeken
  ['www.zoeken.nl'] = {'Zoeken', 'query'},
-- Zoohoo
  ['zoohoo.cz'] = {'Zoohoo', 'q'},
  ['www.zoohoo.cz'] = {'Zoohoo', 'q'},
-- Zoznam
  ['www.zoznam.sk'] = {'Zoznam', 's'}
}

local function find_engine(host, out)	
	local match =	SEARCH_ENGINES[host.host]
	if match then
		local q =  host.params[match[2]]
		if q then
			out.referrer = host.host
			out.query = q
			out.engine = match[1]
		end
	end
end

function get_search_engine(ref)
	local ret = {referrer = ref, query = "", engine = ""}
	
	if ref and ref ~= "" then
		local host = parse(ref)
		
		find_engine(host, ret)
	end
	
	return ret
end
