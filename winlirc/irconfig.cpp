/* 
 * This file is part of the WinLIRC package, which was derived from
 * LIRC (Linux Infrared Remote Control) 0.5.4pre9.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Copyright (C) 1999 Jim Paris <jim@jtan.com>
 */

#include "irconfig.h"
#include "irdriver.h"
#include "stdafx.h"
#include <atlbase.h>
#include "config.h"
#include "remote.h"
#include "globals.h"

CIRConfig::CIRConfig()
{
	CSingleLock lock(&CS_global_remotes,TRUE);

	sense=-1;
	port="COM2";
	global_remotes=NULL;
}

CIRConfig::~CIRConfig()
{
	DEBUG("~CIRConfig\n");
		
	CSingleLock lock(&CS_global_remotes,TRUE);

	if(global_remotes!=NULL)
		free_config(global_remotes);
	
	DEBUG("~CIRConfig done\n");
}

bool CIRConfig::ReadConfig(CIRDriver *driver)
{
	CSingleLock lock(&CS_global_remotes,TRUE);

	CRegKey key;

	if(key.Open(HKEY_LOCAL_MACHINE,"Software\\LIRC")!=ERROR_SUCCESS)
		return false;

	char s[512];
	DWORD len=512;
	if(key.QueryValue(s,"port",&len)!=ERROR_SUCCESS)
		return false;
	port=s;

	DWORD x;
	if(key.QueryValue(x,"sense")!=ERROR_SUCCESS)
		return false;
	sense=(x==2)?-1:x;

	len=512;
	if(key.QueryValue(s,"conf",&len)!=ERROR_SUCCESS)
		return false;
	conf=s;

	FILE *tmp;
	if(conf=="" || (tmp=fopen(conf,"r"))==NULL)
		return false;
		
	if(global_remotes!=NULL)
		free_config(global_remotes);

	global_remotes=read_config(tmp);
	fclose(tmp);

	if(global_remotes==(struct ir_remote *)-1)
	{
		global_remotes=NULL;
		DEBUG("read_config returned -1\n");
		return false;
	}

	/* ??? bad input causes codes to be null, but no */
	/* error is returned from read_config. */
	struct ir_remote *sr;
	for(sr=global_remotes;sr!=NULL;sr=sr->next)
	{
		if(sr->codes==NULL)
		{
			DEBUG("read_config returned remote with null codes\n");
			free_config(global_remotes);

			return false;
		}
	}

	if(global_remotes==NULL)
	{
		DEBUG("read_config returned null\n");
		return false;
	}

	return true;
}

bool CIRConfig::WriteConfig(void)
{
	CRegKey key;

	if(key.Create(HKEY_LOCAL_MACHINE,"Software\\LIRC")!=ERROR_SUCCESS)
		return false;

	if(key.SetValue(port,"port")!=ERROR_SUCCESS)
		return false;

	if(key.SetValue((sense==-1)?2:sense,"sense")!=ERROR_SUCCESS)
		return false;

	if(key.SetValue(conf,"conf")!=ERROR_SUCCESS)
		return false;

	return true;
}