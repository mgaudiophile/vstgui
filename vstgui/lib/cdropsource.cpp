//-----------------------------------------------------------------------------
// VST Plug-Ins SDK
// VSTGUI: Graphical User Interface Framework for VST plugins
//
// Version 4.3
//
//-----------------------------------------------------------------------------
// VSTGUI LICENSE
// (c) 2015, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "cdropsource.h"

namespace VSTGUI {

//-----------------------------------------------------------------------------
CDropSource::CDropEntry::CDropEntry (const void* inBuffer, uint32_t inBufferSize, Type inType)
: type (inType)
{
	buffer.allocate (inBufferSize);
	if (buffer.get ())
		memcpy (buffer.get (), inBuffer, buffer.size ());
}

//-----------------------------------------------------------------------------
CDropSource::CDropEntry::CDropEntry (const CDropEntry& entry)
: type (entry.type)
{
	buffer.allocate (entry.buffer.size ());
	if (buffer.get ())
		memcpy (buffer.get (), entry.buffer.get (), buffer.size ());
}

//-----------------------------------------------------------------------------
CDropSource::CDropEntry::CDropEntry (CDropEntry&& entry) noexcept
{
	buffer = std::move (entry.buffer);
	type = entry.type;
	entry.type = kError;
}

//-----------------------------------------------------------------------------
CDropSource::CDropSource ()
{
}

//-----------------------------------------------------------------------------
CDropSource::CDropSource (const void* buffer, uint32_t bufferSize, Type type)
{
	add (buffer, bufferSize, type);
}

//-----------------------------------------------------------------------------
bool CDropSource::add (const void* buffer, uint32_t bufferSize, Type type)
{
	if (entries.size () == entries.max_size ())
		return false;
	entries.push_back (CDropEntry (buffer, bufferSize, type));
	return true;
}

//-----------------------------------------------------------------------------
uint32_t CDropSource::getCount () const
{
	return static_cast<uint32_t> (entries.size ());
}

//-----------------------------------------------------------------------------
uint32_t CDropSource::getDataSize (uint32_t index) const
{
	return index < getCount () ? static_cast<uint32_t> (entries[index].buffer.size ()) : 0;
}

//-----------------------------------------------------------------------------
CDropSource::Type CDropSource::getDataType (uint32_t index) const
{
	return index < getCount () ? entries[index].type : kError;
}

//-----------------------------------------------------------------------------
uint32_t CDropSource::getData (uint32_t index, const void*& buffer, Type& type) const
{
	if (index >= getCount ())
		return 0;
	buffer = entries[index].buffer.get ();
	type = entries[index].type;
	return static_cast<uint32_t> (entries[index].buffer.size ());
}

//-----------------------------------------------------------------------------
SharedPointer<IDataPackage> CDropSource::create (const void* buffer, uint32_t bufferSize, Type type)
{
	return owned<IDataPackage> (new CDropSource (buffer, bufferSize, type));
}

} // namespace
