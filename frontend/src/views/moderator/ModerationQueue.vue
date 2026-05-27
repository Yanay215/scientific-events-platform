<template>
  <div class="moderator-panel">
    <h2>Управление пользователями (Модератор)</h2>
    <el-table :data="users" style="width: 100%" border stripe>
      <el-table-column prop="id" label="ID" width="80" />
      <el-table-column prop="first_name" label="Имя" />
      <el-table-column prop="last_name" label="Фамилия" />
      <el-table-column prop="role" label="Роль">
        <template #default="scope">
          <el-tag :type="getRoleTag(scope.row.role)">
            {{ scope.row.role }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column label="Действия" width="200">
        <template #default="scope">
          <el-button size="small" @click="editUser(scope.row)">Изменить</el-button>
          <el-button size="small" type="danger" @click="deleteUser(scope.row.id)">Удалить</el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script setup>
import { ref } from 'vue'

// Здесь в будущем будет вызов API (axios/fetch) к вашему C++ бэкенду
const users = ref([
  { id: 1, first_name: 'Иван', last_name: 'Иванов', role: 'participant' },
  { id: 2, first_name: 'Анна', last_name: 'Смирнова', role: 'organizer' }
])

const getRoleTag = (role) => {
  if (role === 'moderator') return 'danger'
  if (role === 'organizer') return 'warning'
  return 'success'
}

const editUser = (user) => { /* Логика открытия el-dialog */ }
const deleteUser = (id) => { /* Логика удаления */ }
</script>