#!/bin/bash
# Script to generate zip file for GTK+ runtime to be included in Pidgin installer

PIDGIN_BASE=`pwd`
PIDGIN_BASE="$PIDGIN_BASE/../../.."
GPG_SIGN=$1

if [ ! -e $PIDGIN_BASE/ChangeLog ]; then
	echo "Pidgin base directory not found"
	exit 1
fi

STAGE_DIR=`readlink -f $PIDGIN_BASE/pidgin/win32/nsis/gtk_runtime_stage`
#Subdirectory of $STAGE_DIR
INSTALL_DIR=Gtk
CONTENTS_FILE=$INSTALL_DIR/CONTENTS
PIDGIN_VERSION=$( < $PIDGIN_BASE/VERSION )

#This needs to be changed every time there is any sort of change.
BUNDLE_VERSION=2.24.14.0
BUNDLE_SHA1SUM="f2f1f74295ab237bedddbe38586bb7e5191178f1"
ZIP_FILE="$PIDGIN_BASE/pidgin/win32/nsis/gtk-runtime-$BUNDLE_VERSION.zip"
#BUNDLE_URL="https://pidgin.im/win32/download_redir.php?version=$PIDGIN_VERSION&gtk_version=$BUNDLE_VERSION&dl_pkg=gtk"
BUNDLE_URL="https://dl.dropbox.com/u/5448886/pidgin-win32/gtk-runtime-2.24.14.0.zip"

function download() {
	if [ -e "$2" ]; then
		echo "File exists"
		exit 1
	fi
	failed=0
	wget -t 5 "$1" -O "$2" -o "wget.log" --retry-connrefused --waitretry=5 \
		--ca-certificate="${STAGE_DIR}/../cacert.pem" \
		|| failed=1
	if [ $failed != 0 ] ; then
		if [ "$3" != "quiet" ] ; then
			echo "Download failed"
			cat "wget.log"
		fi
		rm "wget.log"
		rm -f "$2"
		return 1
	fi
	rm "wget.log"
	return 0
}

cat $PIDGIN_BASE/share/ca-certs/*.pem > $STAGE_DIR/../cacert.pem

#Download the existing file (so that we distribute the exact same file for all releases with the same bundle version)
FILE="$ZIP_FILE"
if [ ! -e "$FILE" ]; then
	echo "Downloading the existing file"
	download "$BUNDLE_URL" "$FILE" "quiet"
fi
if [ -e "$FILE" ]; then
	CHECK_SHA1SUM=`sha1sum $FILE`
	CHECK_SHA1SUM=${CHECK_SHA1SUM%%\ *}
else
	CHECK_SHA1SUM=
fi
if [ "$CHECK_SHA1SUM" != "$BUNDLE_SHA1SUM" ]; then
	if [ "x$CHECK_SHA1SUM" != "x" ]; then
		echo "sha1sum ($CHECK_SHA1SUM) for $FILE doesn't match expected value of $BUNDLE_SHA1SUM"
	fi
	# Allow "devel" versions to build their own bundles if the download doesn't succeed
	if [[ "$PIDGIN_VERSION" == *"devel" ]]; then
		echo "Continuing GTK+ Bundle creation for development version of Pidgin"
	else
		exit 1
	fi
else
	exit 0
fi

#DOWNLOAD_HOST="http://download.opensuse.org/repositories/windows:/mingw:/win32/openSUSE_12.3/noarch/"
#TODO: this is just a temporary mirror - Tomek Wasilczyk's <tomkiewicz@cpw.pidgin.im> Dropbox
DOWNLOAD_HOST="https://dl.dropbox.com/u/5448886/pidgin-win32/runtime-deps/"

ATK="${DOWNLOAD_HOST}mingw32-atk-2.6.0-1.4.noarch.rpm ATK 2.6.0-1.4 sha1sum:d0792a3355b22cf4f0e218382dde71b1e22a2b03"
CAIRO2="${DOWNLOAD_HOST}mingw32-libcairo2-1.10.2-8.4.noarch.rpm Cairo 1.10.2-8.4 sha1sum:f69af74753c7fcd95b7778eee7c3d731d64749ba"
DBUS="${DOWNLOAD_HOST}mingw32-dbus-1-1.7.0-2.1.noarch.rpm D-Bus 1.7.0-2.1 sha1sum:6961e4df15172b057e91b266dbb7cbd01a736e56"
DBUS_GLIB="${DOWNLOAD_HOST}mingw32-dbus-1-glib-0.92-3.4.noarch.rpm dbus-glib 0.92-3.4 sha1sum:3af1dd35cbe2cdf62ee5144862934f5f8dd5e20d"
ENCHANT="${DOWNLOAD_HOST}mingw32-enchant-1.6.0-3.7.noarch.rpm Enchant 1.6.0-3.4 sha1sum:f7e0571ef98833b087be6c9d71008b3c4c4435d6"
FONTCONFIG="${DOWNLOAD_HOST}mingw32-fontconfig-2.10.92-1.2.noarch.rpm fontconfig 2.10.92-1.2 sha1sum:d8da351f5d4d9816f02ac5287dd7887c711902ed"
FREETYPE="${DOWNLOAD_HOST}mingw32-freetype-2.4.11-2.1.noarch.rpm freetype 2.4.11-2.1 sha1sum:0eee29696e87d47e7487ba5e90530bf20159f31e"
GDK_PIXBUF="${DOWNLOAD_HOST}mingw32-gdk-pixbuf-2.28.0-1.1.noarch.rpm gdk-pixbuf 2.28.0-1.1 sha1sum:f75f3c989d8d8ee3f92555d269465d1bc26ea428"
GEOCLUE="${DOWNLOAD_HOST}mingw32-libgeoclue-0.12.99-1.4.noarch.rpm Geoclue 0.12.99-1.4 sha1sum:cf3df30e75c5b38fbe2f63689cadcc2930823b9c"
GLIB="${DOWNLOAD_HOST}mingw32-glib2-2.36.0-2.2.noarch.rpm Glib 2.36.0-2.2 sha1sum:1b788be9f7b15cde9452f135831feebc51f3a5a4"
GST="${DOWNLOAD_HOST}mingw32-libgstreamer-0.10.35-1.4.noarch.rpm GStreamer 0.10.35-1.4 sha1sum:fd5bb6f8a9083eb3ca402670e7c38474f7270efe"
GST_INT="${DOWNLOAD_HOST}mingw32-libgstinterfaces-0.10.32-5.4.noarch.rpm GStreamer-interfaces 0.10.32-5.4 sha1sum:cec1dd36bbcc10716e9f8776e4bd53fb0b07d8bb"
GTK2="${DOWNLOAD_HOST}mingw32-gtk2-2.24.14-1.4.noarch.rpm GTK+ 2.24.14-1.4 sha1sum:71971fe63d355aa893536b691f249ace78d89a2b"
GTKSPELL="${DOWNLOAD_HOST}mingw32-gtkspell-2.0.16-2.5.noarch.rpm GtkSpell 2.0.16-2.5 sha1sum:49381274b248ffe56ef733a90908541dc587c9af"
LIBFFI="${DOWNLOAD_HOST}mingw32-libffi-3.0.10-2.4.noarch.rpm libffi 3.0.10-2.4 sha1sum:871f13d5f02c03d62b882cc1fa4c98dcff76d4c5"
LIBGCC="${DOWNLOAD_HOST}mingw32-libgcc-4.8.0-6.1.noarch.rpm libgcc 4.8.0-6.1 sha1sum:ab599bf07bf2d56367c57b442440598358c943af"
LIBHB="${DOWNLOAD_HOST}mingw32-libharfbuzz0-0.9.14-1.5.noarch.rpm libharfbuzz 0.9.14-1.5 sha1sum:54106eb1e8ff42c79ea52c1ef64a066f1f2cb133"
LIBJASPER="${DOWNLOAD_HOST}mingw32-libjasper-1.900.1-6.4.noarch.rpm JasPer 1.900.1-6.4 sha1sum:25e01ed277b8dda6191afb7dd0e0928558c1f2d6"
LIBICU="${DOWNLOAD_HOST}mingw32-libicu-51.1-2.1.noarch.rpm ICU 51.1-2.1 sha1sum:97ec8264e38abceeadda4631730bb0b5f3f3ebfe"
LIBINTL="${DOWNLOAD_HOST}mingw32-libintl-0.18.1.1-13.4.noarch.rpm libintl 0.18.1.1-13.4 sha1sum:043c3b8eb9c872681faed5ec5263456a24bf29e4"
LIBJPEG="${DOWNLOAD_HOST}mingw32-libjpeg-8d-3.4.noarch.rpm libjpeg 8d-3.4 sha1sum:5d1db1eaabc6ababbed648408adbbe6eee0292fc"
LIBJSON="${DOWNLOAD_HOST}mingw32-libjson-glib-0.14.2-1.4.noarch.rpm json-glib 0.14.2-1.4 sha1sum:698194c97baf732bd6b109778f2834d71eedc524"
LIBLZMA="${DOWNLOAD_HOST}mingw32-liblzma-5.0.4-1.4.noarch.rpm liblzma 5.0.4-1.4 sha1sum:ef360fab4b6c77d1618891ccc8f52c2421f37c09"
LIBPNG="${DOWNLOAD_HOST}mingw32-libpng-1.5.11-1.4.noarch.rpm libpng 1.5.11-1.4 sha1sum:99d0a077f70e83f7df10f28915a2137a0ff34462"
LIBSOUP="${DOWNLOAD_HOST}mingw32-libsoup-2.40.3-1.9.noarch.rpm libsoup 2.40.3-1.9 sha1sum:d343f6f0661c4527901a0ba446f00af2f925e70f"
LIBSSP="${DOWNLOAD_HOST}mingw32-libssp-4.8.0-6.1.noarch.rpm LibSSP 4.8.0-6.1 sha1sum:c05b2e0470f41d26f8ebfff93dfd51263842a4ea"
LIBSTDCPP="${DOWNLOAD_HOST}mingw32-libstdc++-4.8.0-6.1.noarch.rpm libstdc++ 4.8.0-6.1 sha1sum:627860950e951764fe1aa229d3a63bb01618ba90"
LIBTIFF="${DOWNLOAD_HOST}mingw32-libtiff-4.0.2-1.4.noarch.rpm libtiff 4.0.2-1.4 sha1sum:9a8f8b018e8bafd47067fe6fd0debc1e887239b1"
LIBXML="${DOWNLOAD_HOST}mingw32-libxml2-2.9.0-2.1.noarch.rpm libxml 2.9.0-2.1 sha1sum:de73090544effcd167f94fcfe8e2d1f005adbea7"
LIBXSLT="${DOWNLOAD_HOST}mingw32-libxslt-1.1.28-1.1.noarch.rpm libxslt 1.1.28-1.1 sha1sum:b43448edbbb04d72e355bd45d5bee94414395589"
MEANW="${DOWNLOAD_HOST}mingw32-meanwhile-1.0.2-3.1.noarch.rpm Meanwhile 1.0.2-3.1 sha1sum:89fc8c6c15bf4825a7d750a15134f14c8ae05c9c"
MOZNSS="${DOWNLOAD_HOST}mingw32-mozilla-nss-3.14.3-2.1.noarch.rpm NSS 3.14.3-2.1 sha1sum:0eec79ddee11f2453af34f85c5939e6d33f6c0c5"
MOZNSPR="${DOWNLOAD_HOST}mingw32-mozilla-nspr-4.9.6-3.1.noarch.rpm NSPR 4.9.6-3.1 sha1sum:87b562845f8e46d44e57262752dae406620ae6df"
PANGO="${DOWNLOAD_HOST}mingw32-pango-1.34.0-2.1.noarch.rpm Pango 1.34.0-2.1 sha1sum:f382abb3dfb822f647a902266039571c0506bfa4"
PIXMAN="${DOWNLOAD_HOST}mingw32-pixman-0.26.0-1.4.noarch.rpm pixman 0.26.0-1.4 sha1sum:f751fe428ea83996daf7e57bff6f4f79361b0d29"
PTHREADS="${DOWNLOAD_HOST}mingw32-pthreads-2.8.0-14.6.noarch.rpm pthreads 2.8.0-14.6 sha1sum:e948ae221f82bbcb4fbfd991638e4170c150fe9f"
SQLITE="${DOWNLOAD_HOST}mingw32-libsqlite-3.7.6.2-1.6.noarch.rpm SQLite 3.7.6.2-1.6 sha1sum:f61529bc0c996d9af28a94648ce6102d579ed928"
TCL="${DOWNLOAD_HOST}mingw32-tcl-8.5.9-13.6.noarch.rpm Tcl 8.5.9-13.6 sha1sum:442eaf5e761d59bb527cb1f9f93749ebc13b4367"
TK="${DOWNLOAD_HOST}mingw32-tk-8.5.9-8.6.noarch.rpm Tk 8.5.9-8.6 sha1sum:d0884c56f767f76981ddf7bc6d74d3c47237740a"

#webkit 1.10 crashes when calling document.createElement, so I grabbed 1.8 from openSUSE_Factory instead
#TODO: investigate it
#WEBKITGTK="${DOWNLOAD_HOST}mingw32-libwebkitgtk-1.10.2-1.3.noarch.rpm WebKitGTK+ 1.10.2-1.3 sha1sum:33b558d2110fc2caf2c3c0ab24a6c18645814893"
WEBKITGTK="${DOWNLOAD_HOST}mingw32-libwebkitgtk-1.8.3-1.14.noarch.rpm WebKitGTK+ 1.8.3-1.14 sha1sum:ade86455fc2da257f4fe5831367f500a61a1af9a"

ZLIB="${DOWNLOAD_HOST}mingw32-zlib-1.2.7-1.4.noarch.rpm zlib 1.2.7-1.4 sha1sum:83e91f3b4d14e47131ca33fc69e12b82aabdd589"
ALL="ATK CAIRO2 DBUS DBUS_GLIB ENCHANT FONTCONFIG FREETYPE GDK_PIXBUF GEOCLUE GLIB GST GST_INT GTK2 GTKSPELL LIBFFI LIBGCC LIBHB LIBJASPER LIBICU LIBINTL LIBJPEG LIBJSON LIBLZMA LIBPNG LIBSOUP LIBSSP LIBSTDCPP LIBTIFF LIBXML LIBXSLT MEANW MOZNSS MOZNSPR PANGO PIXMAN PTHREADS SQLITE TCL TK WEBKITGTK ZLIB"

mkdir -p $STAGE_DIR
cd $STAGE_DIR

rm -rf $INSTALL_DIR
mkdir $INSTALL_DIR

#new CONTENTS file
echo Bundle Version $BUNDLE_VERSION > $CONTENTS_FILE

#TODO: temporary mirror also
CPIO_URL="https://dl.dropbox.com/u/5448886/pidgin-win32/cpio/bsdcpio-3.0.3-1.4.tar.gz"
CPIO_SHA1SUM="0460c7a52f8c93d3c4822d6d1aaf9410f21bd4da"
CPIO_DIR="bsdcpio"
FILE="bsdcpio.tar.gz"
if [ ! -e "$FILE" ]; then
	echo "Downloading bsdcpio"
	download "$CPIO_URL" "$FILE" || exit 1
fi
CHECK_SHA1SUM=`sha1sum $FILE`
CHECK_SHA1SUM=${CHECK_SHA1SUM%%\ *}
if [ "$CHECK_SHA1SUM" != "$CPIO_SHA1SUM" ]; then
	echo "sha1sum ($CHECK_SHA1SUM) for $FILE doesn't match expected value of $CPIO_SHA1SUM"
	rm $FILE
	exit 1
fi
rm -rf "$CPIO_DIR"
mkdir "$CPIO_DIR"
tar xf "$FILE" --strip-components=1 --directory="$CPIO_DIR" || exit 1

function download_and_extract {
	URL=${1%%\ *}
	VALIDATION=${1##*\ }
	NAME=${1%\ *}
	NAME=${NAME#*\ }
	FILE=$(basename $URL)
	MINGW_DIR="usr/i686-w64-mingw32/sys-root/mingw"
	MINGW_DIR_TOP="usr"
	if [ ! -e $FILE ]; then
		echo "Downloading $NAME"
		download "$URL" "$FILE" || exit 1
	fi
	VALIDATION_TYPE=${VALIDATION%%:*}
	VALIDATION_VALUE=${VALIDATION##*:}
	if [ $VALIDATION_TYPE == 'sha1sum' ]; then
		CHECK_SHA1SUM=`sha1sum $FILE`
		CHECK_SHA1SUM=${CHECK_SHA1SUM%%\ *}
		if [ "$CHECK_SHA1SUM" != "$VALIDATION_VALUE" ]; then
			echo "sha1sum ($CHECK_SHA1SUM) for $FILE doesn't match expected value of $VALIDATION_VALUE"
			exit 1
		fi
	elif [ $VALIDATION_TYPE == 'gpg' ]; then
		if [ ! -e "$FILE.asc" ]; then
			echo Downloading GPG key for $NAME
			download "$URL.asc" "$FILE.asc" || exit 1
		fi
		#Use our own keyring to avoid adding stuff to the main keyring
		#This doesn't use $GPG_SIGN because we don't this validation to be bypassed when people are skipping signing output
		GPG_BASE="gpg -q --keyring $STAGE_DIR/$VALIDATION_VALUE-keyring.gpg" 
		if [[ ! -e $STAGE_DIR/$VALIDATION_VALUE-keyring.gpg \
				|| `$GPG_BASE --list-keys "$VALIDATION_VALUE" > /dev/null && echo -n "0"` -ne 0 ]]; then
			touch $STAGE_DIR/$VALIDATION_VALUE-keyring.gpg
		       	$GPG_BASE --no-default-keyring --keyserver pgp.mit.edu --recv-key "$VALIDATION_VALUE" || exit 1
		fi
		$GPG_BASE --verify "$FILE.asc" || (echo "$FILE failed signature verification"; exit 1) || exit 1
	else
		echo "Unrecognized validation type of $VALIDATION_TYPE"
		exit 1
	fi
	EXTENSION=${FILE##*.}
	#This is an OpenSuSE build service RPM
	if [ $EXTENSION == 'rpm' ]; then
		rm -rf $MINGW_DIR_TOP
		bsdcpio/bsdcpio.exe --quiet -f etc/fonts/conf.d -di < $FILE || exit 1
		cp -rf $MINGW_DIR/* $INSTALL_DIR
		rm -rf $MINGW_DIR_TOP
	else
		unzip -q $FILE -d $INSTALL_DIR || exit 1
	fi
	echo "$NAME" >> $CONTENTS_FILE
}

echo "Downloading and extracting components..."
for VAL in $ALL
do
	VAR=${!VAL}
	download_and_extract "$VAR"
done
mv "${STAGE_DIR}/${INSTALL_DIR}/share/tcl8.5" "${STAGE_DIR}/${INSTALL_DIR}/lib/"
rm -rf $CPIO_DIR
rm "${STAGE_DIR}/../cacert.pem"

echo "All components ready"

#Default GTK+ Theme to MS-Windows (already set)
#echo gtk-theme-name = \"MS-Windows\" > $INSTALL_DIR/etc/gtk-2.0/gtkrc

#Blow away translations that we don't have in Pidgin (temporarily not included)
#for LOCALE_DIR in $INSTALL_DIR/share/locale/*
#do
#	LOCALE=$(basename $LOCALE_DIR)
#	if [ ! -e $PIDGIN_BASE/po/$LOCALE.po ]; then
#		echo Removing $LOCALE translation as it is missing from Pidgin
#		rm -r $LOCALE_DIR
#	fi
#done

#Generate zip file to be included in installer
rm -f $ZIP_FILE
zip -9 -r $ZIP_FILE Gtk

if [ "`$GPG_SIGN -K 2> /dev/null`" != "" ]; then
	($GPG_SIGN -ab $ZIP_FILE && $GPG_SIGN --verify $ZIP_FILE.asc) || exit 1
else
	echo "Warning: cannot sign generated bundle"
fi

exit 0
