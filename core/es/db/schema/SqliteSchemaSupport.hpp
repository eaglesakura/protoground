#pragma once

#include    "es/protoground.hpp"
#include    "SqliteEntity.h"
#include    "SqliteSchema.h"


/**
 * 1つのエンティティを作成するためのマクロ
 */
#define PGD_SQLITE_ENTITY_MAKE(dataStruct_type)     ::es::SqliteEntity entity_##dataStruct_type(#dataStruct_type);  dataStruct_type __SAMPLE__##dataStruct_type

/**
 * プロパティを追加するためのマクロ
 */
#define PGD_SQLITE_ENTITY_PROPERTY(dataStruct_type, dataStruct_prop_name, dataStruct_prop_flag)                   \
                                                        (entity_##dataStruct_type).addProperty( ::es::SqliteEntity::Property( \
                                                                                                        #dataStruct_prop_name, ::es::SqliteEntity::Property::toType(__SAMPLE__##dataStruct_type.dataStruct_prop_name), dataStruct_prop_flag),     \
                                                                                                        &__SAMPLE__##dataStruct_type, &(__SAMPLE__##dataStruct_type.dataStruct_prop_name))

#define PGD_SQLITE_SCHEMA  __SCHEMA__

/**
 * Schemaを生成する
 */
#define PGD_SQLITE_SCHEMA_MAKE(version) ::es::SqliteSchema  PGD_SQLITE_SCHEMA(version)


/**
 * Schemaを生成する
 */
#define PGD_SQLITE_SCHEMA_REQUEST_INITIALIZE (!PGD_SQLITE_SCHEMA.hasEntity())

/**
 * SchemaにEntityを追加する
 */
#define PGD_SQLITE_SCHEMA_ADD_ENTITY(dataStruct_type)   PGD_SQLITE_SCHEMA.addEntity(entity_##dataStruct_type)
