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

#pragma myproof instrumente (func1, func2, func3)
#pragma myproof instrumente (func4, func5)
#pragma myproof instrumente func6
#pragma myproof instrumente (func11, func111)
#pragma myproof instrumente (func21, func22)

void func111() { usleep(100000); }
void func11() { usleep(5000); func111(); }
void func1() { usleep(10000); func11(); }

void func22() { usleep(3000); }
void func21() { usleep(3000); }
void func2() { usleep(1000); func21(); func22(); }

void func3() { usleep(5000); }
void func4() { usleep(1000); }
void func5() { usleep(2000); }
void func6() { usleep(3); }

int main(void)
{
    func1();
    func2();
    func3();
    func4();
    func5();
    func6();

    return 0;
}
