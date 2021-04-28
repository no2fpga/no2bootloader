#!/usr/bin/env bash
# jenkins build helper script for no2bootloader.  This is how we build on jenkins.osmocom.org
#
# environment variables:
# * WITH_MANUALS: build manual PDFs if set to "1"
# * PUBLISH: upload manuals after building if set to "1" (ignored without WITH_MANUALS = "1")

if ! [ -x "$(command -v osmo-build-dep.sh)" ]; then
	echo "Error: We need to have scripts/osmo-deps.sh from http://git.osmocom.org/osmo-ci/ in PATH !"
	exit 2
fi

set -e

TOPDIR=`pwd`
publish="$1"

base="$PWD"
deps="$base/deps"
inst="$deps/install"
export deps inst

export PKG_CONFIG_PATH="$inst/lib/pkgconfig:$PKG_CONFIG_PATH"
export LD_LIBRARY_PATH="$inst/lib"
export PATH="$inst/bin:$PATH"

osmo-clean-workspace.sh

mkdir "$deps" || true

# we assume that PATH includes the path to the respective toolchain

# different boards for which we build
BOARDS="icebreaker bitsy_v0 bitsy_v1 ice1usb icepick e1tracer fomu_hacker fomu_pvt1"

cat > "/build/known_hosts" <<EOF
[ftp.osmocom.org]:48 ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDDgQ9HntlpWNmh953a2Gc8NysKE4orOatVT1wQkyzhARnfYUerRuwyNr1GqMyBKdSI9amYVBXJIOUFcpV81niA7zQRUs66bpIMkE9/rHxBd81SkorEPOIS84W4vm3SZtuNqa+fADcqe88Hcb0ZdTzjKILuwi19gzrQyME2knHY71EOETe9Yow5RD2hTIpB5ecNxI0LUKDq+Ii8HfBvndPBIr0BWYDugckQ3Bocf+yn/tn2/GZieFEyFpBGF/MnLbAAfUKIdeyFRX7ufaiWWz5yKAfEhtziqdAGZaXNaLG6gkpy3EixOAy6ZXuTAk3b3Y0FUmDjhOHllbPmTOcKMry9
[ftp.osmocom.org]:48 ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBPdWn1kEousXuKsZ+qJEZTt/NSeASxCrUfNDW3LWtH+d8Ust7ZuKp/vuyG+5pe5pwpPOgFu7TjN+0lVjYJVXH54=
[ftp.osmocom.org]:48 ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIK8iivY70EiR5NiGChV39gRLjNpC8lvu1ZdHtdMw2zuX
EOF

SSH_COMMAND="ssh -o 'UserKnownHostsFile=/build/known_hosts' -p 48"

for b in $BOARDS; do
	echo
	echo "=============== FIRMWARE $b =============="
	make -C firmware clean all BOARD=$b

	echo
	echo "=============== UPLOAD FIRMWARE $b =============="
	# The argument '--publish' is used to trigger publication/upload of firmware
	if [ "x$publish" = "x--publish" ]; then
		rsync --archive --verbose --compress --rsh "$SSH_COMMAND" \
			$TOPDIR/firmware/no2bootloader-$b-*-*.{bin,elf} \
			binaries@ftp.osmocom.org:web-files/no2bootloader/$b/all/
		rsync --archive --copy-links --verbose --compress --rsh "$SSH_COMMAND" \
			$TOPDIR/firmware/no2bootloader-$b.{bin,elf} \
			binaries@ftp.osmocom.org:web-files/no2bootloader/$b/latest/
	fi
done

osmo-clean-workspace.sh
