/***********************************************************************

 Copyright (c) 2008, Memo Akten, www.memo.tv

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ***********************************************************************/

#include "ofxObjCPointer.h"
using Poco::ScopedLock;

ofxObjCPointer::ofxObjCPointer() {
	mutex = new ofMutex;
    ScopedLock<ofMutex> lock(*mutex);
	_useCountOfThisObject = 1;
}

ofxObjCPointer::~ofxObjCPointer() {
    ScopedLock<ofMutex> lock(*mutex);
	delete mutex;
}

void ofxObjCPointer::retain() {
    ScopedLock<ofMutex> lock(*mutex);
	_useCountOfThisObject++;

}

void ofxObjCPointer::release() {
    ScopedLock<ofMutex> lock(*mutex);
	_useCountOfThisObject--;
	if(_useCountOfThisObject == 0) {
		delete this;
	}
}
