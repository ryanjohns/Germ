#include "TreeColumn.h"

TreeColumn::TreeColumn() {
	add(m_col_label);
	add(m_col_sort);
	add(m_artist_node);
	add(m_album_node);
}

TreeColumn::~TreeColumn() {}
