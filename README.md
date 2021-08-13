<h1 align="center">Hızlı Pedallı/Bisiklet Araba Yarışı Oyun Makinesi </h1>
<p  align="center">
    <img src="./img/hizli_pedallli_baslik.png" width="70%" height="70%" border: 5px solid #555 >
</p>

## İçerik

- [Giriş](#Giriş)
- [Çalışma Şekli](#Çalışma-Şekli)
- [Kullanılan Teknolojiler](#Kullanılan-Teknolojiler)
- [İletişim](#İletişim)

## Giriş

 Bu oyun makinesi iki kişi veya tek olarak oynanabilir. Tek kişi oynarken oyuncu sistemle yarışır. İki kişilik oyunda ise oyuncular birbiri ile yarışır. İki adet oyuncak araba kendileri ile ilişkilendirilmiş bisikletlerin hızıyla orantılı bir şekilde pistte hareket ederler. Anlık ortalama hız, alınan yol, tur sayısı ve oyun süresi RGB led panel ekranlarda gösterilir. Oyun süresi sonunda en çok yol giden oyuncu kazanır.

## Çalışma Şekli

Sistemi mikrodenetleyici tabanlı bir ana kart kontrol etmektedir. Oyun başlatmak için jeton atılır veya temassız kart kart, kart okuyucuya okutulur. Bisiklet tekerlerine bağlı olan iki adet sensörden gelen bilgi mcu tarafından okunarak, gerekli hesaplamalar yapılıp hız ve yol bilgisine çevrilir. Bilgiler seri olarak arduino megaya gönderilir. RGB led paneller üzerinde ayrı ayrı gösterilir. Arduino megalar RGB led panelleri sürmek için kullanılmıştır. 

<p  align="center">
    <img src="./pcb/bisiklet_araba_yarisi_pcb.png" width="90%" height="90%" border: 5px solid #555 >
</p

 Oyun süresi, çalışması için gerekli olan jeton miktarı vb. ayarlar kart üzerindeki LCD den yapılmaktadır.

Oyun, oyun müziği eşliğinde oynanır. 

<br>
<p  align="center">
    <img src="./pcb/ardu_mega_serial_shield.png" width="80%" height="75%" border: 5px solid #555 >

</p>

## Kullanılan Teknolojiler

```bash
- PIC18F45K22 mikrodenetleyici.
- CCS PIC C Derleyici
- Hız algılama.
- Motor yön ve hız kontrolü.
- PCB devre şeması çizme ve oluşturma.

```

## Örnek çalışma videosu :

[![](https://camo.githubusercontent.com/241d4106ff5edca2ee25e04dcf4546fad9d20b626f7a10990307e8f83e95459f/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f796f75747562652d2532334646303030302e7376673f267374796c653d666f722d7468652d6261646765266c6f676f3d796f7574756265266c6f676f436f6c6f723d7768697465253232)](https://youtu.be/KFKP7lpa3pI)

## İletişim

- GitHub [@your-ilyas9461](https://github.com/ilyas9461)
- Linkedin [@your-linkedin](https://www.linkedin.com/in/ilyas-yağcioğlu-6a6b17217)

### *** Not : 
Devre endüstriyel olarak üretimde olduğu için PCB devre şemaları paylaşılmamıştır.
