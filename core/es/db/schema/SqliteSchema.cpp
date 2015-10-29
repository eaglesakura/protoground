#include "SqliteSchema.h"

namespace es {

const SqliteEntity &SqliteSchema::getMetaEntity() {
    static SqliteEntity entity;
    if (entity.properties.empty()) {
        entity.name = "PGD_META";
        Meta meta;

        {
            SqliteEntity::Property prop;
            prop.name = "KEY";
            prop.type = SqliteEntity::Property::Type_String;
            prop.flags = SqliteEntity::Property::Flag_PrimaryKey;
            entity.properties.push_back(prop);

            entity.binds.push_back(SqliteEntity::Bind::make(prop, &meta, &meta.KEY));
        }
        {
            SqliteEntity::Property prop;
            prop.name = "VALUE";
            prop.type = SqliteEntity::Property::Type_String;
            prop.flags = SqliteEntity::Property::Flag_NotNull;
            entity.properties.push_back(prop);

            entity.binds.push_back(SqliteEntity::Bind::make(prop, &meta, &meta.VALUE));
        }

    }
    return entity;
}

SqliteSchema &SqliteSchema::addEntity(const SqliteEntity &entity) {
    entities.push_back(entity);
    return *this;
}

bool SqliteSchema::hasEntity() const {
    return entities.size() > 0;
}
}