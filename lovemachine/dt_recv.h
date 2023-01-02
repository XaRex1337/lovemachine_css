#pragma once
#include "includes.h"
#include "definitions.h"

class RecvTable;
class CRecvProxyData;
class RecvProp;

typedef void (*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

// NOTE: DataTable receive proxies work differently than the other proxies.
// pData points at the object + the recv table's offset.
// pOut should be set to the location of the object to unpack the data table into.
// If the parent object just contains the child object, the default proxy just does *pOut = pData.
// If the parent object points at the child object, you need to dereference the pointer here.
// NOTE: don't ever return null from a DataTable receive proxy function. Bad things will happen.
//typedef void (*DataTableRecvVarProxyFn)(const RecvProp* pProp, void** pOut, void* pData, int objectID);

#define FMTFUNCTION( a, b )
int V_snprintf(char* pDest, int destLen, const char* pFormat, ...) FMTFUNCTION(3, 4);

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,	// An array of the base types (can't be of datatables).
	DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	DPT_Quaternion,
#endif

//#ifdef SUPPORTS_INT64
	DPT_Int64,
//#endif

	DPT_NUMSendPropTypes

} SendPropType;

class DVariant
{
public:
	DVariant() { m_Type = DPT_Float; }
	DVariant(float val) { m_Type = DPT_Float; m_Float = val; }

	const char* ToString()
	{
		static char text[128];

		switch (m_Type)
		{
		case DPT_Int:
			V_snprintf(text, sizeof(text), "%i", m_Int);
			break;
		case DPT_Float:
			V_snprintf(text, sizeof(text), "%.3f", m_Float);
			break;
		case DPT_Vector:
			V_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2]);
			break;
		case DPT_VectorXY:
			V_snprintf(text, sizeof(text), "(%.3f,%.3f)",
				m_Vector[0], m_Vector[1]);
			break;
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
		case DPT_Quaternion:
			V_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f %.3f)",
				m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3]);
			break;
#endif
		case DPT_String:
			if (m_pString)
				return m_pString;
			else
				return "NULL";
			break;
		case DPT_Array:
			V_snprintf(text, sizeof(text), "Array");
			break;
		case DPT_DataTable:
			V_snprintf(text, sizeof(text), "DataTable");
			break;
//#ifdef SUPPORTS_INT64
		case DPT_Int64:
			V_snprintf(text, sizeof(text), "%I64d", m_Int64);
			break;
//#endif
		default:
			V_snprintf(text, sizeof(text), "DVariant type %i unknown", m_Type);
			break;
		}

		return text;
	}

	union
	{
		float	m_Float;
		int		m_Int;
		const char* m_pString;
		void* m_pData;	// For DataTables.
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
		float	m_Vector[4];
#else
		float	m_Vector[3];
#endif

//#ifdef SUPPORTS_INT64
		__int64	m_Int64;
//#endif
	};
	SendPropType	m_Type;
};

class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp;		// The property it's receiving.

	DVariant		m_Value;			// The value given to you to store.

	int				m_iElement;			// Which array element you're getting.

	int				m_ObjectID;			// The object being referred to.
};

class RecvProp
{
	// This info comes from the receive data table.
public:

	const char* m_pVarName;
	SendPropType			m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;


//private:

	bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

	// Extra data that certain special property types bind to the property here.
	const void* m_pExtraData;

	// If this is an array (DPT_Array).
	RecvProp* m_pArrayProp;
	sample_fn	m_ArrayLengthProxy;

	RecvVarProxyFn			m_ProxyFn;
	sample_fn	m_DataTableProxyFn;	// For RDT_DataTable.

	RecvTable* m_pDataTable;		// For RDT_DataTable.
	int						m_Offset;

	int						m_ElementStride;
	int						m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* m_pParentArrayPropName;
};


class RecvTable
{
public:

	// Properties described in a table.
	RecvProp* m_pProps;
	int				m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	sample_class* m_pDecoder;

	const char* m_pNetTableName;	// The name matched between client and server.


private:

	bool			m_bInitialized;
	bool			m_bInMainList;
};
