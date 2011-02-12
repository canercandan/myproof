// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Authors:
 * Caner Candan <caner@candan.fr>, http://caner.candan.fr
 * Aurèle Mahéo <aurele.maheo@gmail.com>
 */

#pragma myproof test "string"

int main()
{
#pragma myproof test "another string" //warning ignoring
#pragma myproof test 12 //warning ignoring
#pragma myproof test tata //warning ignoring
#pragma myproof toto //warning ignoring

#pragma myproof instrumente foo ffff // two NAME instead one
#pragma myproof instrumente (func1, func2,,) // no CLOSE_PARA after last NAME
#pragma myproof instrumente (func1, func2 // no CLOSE_PARA

    return 0;
}
