#!/bin/bash
#   Copyright (C) 2012~2012 by Yichao Yu
#   yyc1992@gmail.com
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.

action="$1"

shift 1 || exit 1

. "$(dirname "${BASH_SOURCE}")/fcitx-write-po.sh"

case "${action}" in
    -c)
        in_file="${1}"
        if fcitx_str_match "*.rc" "${in_file}"; then
            exit 0
        elif fcitx_str_match "*.kcfg" "${in_file}"; then
            exit 0
        elif fcitx_str_match "*.ui" "${in_file}"; then
            exit 0
        fi
        exit 1
        ;;
    -w)
        out_file="${1}"
        shift || exit 1
        [[ -z "$*" ]] && exit 1
        echo "Extracting po string from kde sources."
        # need to touch source dir here since lupdate will otherwise include
        # absolute path (or wrong relative path) in po files afaik.
        tempfile="$(mktemp --suffix=_fcitx_kde_$$.cpp)"
        {
            [[ -z "$*" ]] || extractrc "$@"
            echo 'i18nc("NAME OF TRANSLATORS", "Your names");'
            echo 'i18nc("EMAIL OF TRANSLATORS", "Your emails");'
        } > "${tempfile}"
        xgettext --from-code=UTF-8 -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 \
            -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 -kI18N_NOOP:1 \
            -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 \
            -kki18np:1,2 -kki18ncp:1c,2,3 -c --no-location "${tempfile}" \
            -o "${out_file}"
        rm "${tempfile}"
        fcitx_fix_po_charset_utf8 "${out_file}"
        exit 0
        ;;
esac
