<template>
  <div class="moderator-panel">
    <h2>Управление пользователями (Модератор)</h2>

    <el-table :data="users" v-loading="loading" style="width: 100%" border stripe empty-text="Пользователи не найдены">
      <el-table-column prop="email" label="Email" min-width="200" />
      <el-table-column label="Имя" min-width="160">
        <template #default="scope">
          {{ scope.row.last_name }} {{ scope.row.first_name }}
        </template>
      </el-table-column>
      <el-table-column prop="role" label="Роль" width="150">
        <template #default="scope">
          <el-tag :type="getRoleTag(scope.row.role)">
            {{ roleLabel(scope.row.role) }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column label="Статус" width="130">
        <template #default="scope">
          <el-tag :type="scope.row.is_blocked ? 'danger' : 'success'">
            {{ scope.row.is_blocked ? 'Заблокирован' : 'Активен' }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column label="Действия" width="260">
        <template #default="scope">
          <el-button size="small" @click="openRoleDialog(scope.row)">Изменить роль</el-button>
          <el-button
            size="small"
            :type="scope.row.is_blocked ? 'success' : 'danger'"
            @click="toggleBlock(scope.row)"
          >
            {{ scope.row.is_blocked ? 'Разблокировать' : 'Ограничить доступ' }}
          </el-button>
        </template>
      </el-table-column>
    </el-table>

    <el-dialog v-model="roleDialogVisible" title="Изменение роли пользователя" width="420px">
      <p v-if="selectedUser" class="dialog-user">{{ selectedUser.email }}</p>
      <el-select v-model="newRole" placeholder="Выберите роль" style="width: 100%">
        <el-option label="Участник" value="participant" />
        <el-option label="Организатор" value="organizer" />
        <el-option label="Модератор" value="moderator" />
      </el-select>
      <template #footer>
        <el-button @click="roleDialogVisible = false">Отмена</el-button>
        <el-button type="primary" :loading="saving" @click="saveRole">Сохранить</el-button>
      </template>
    </el-dialog>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { getModeratorUsers, updateUserRole, setUserBlocked } from '@/services/api'

const users = ref([])
const loading = ref(false)
const saving = ref(false)

const roleDialogVisible = ref(false)
const selectedUser = ref(null)
const newRole = ref('')

const roleLabels = {
  participant: 'Участник',
  organizer: 'Организатор',
  moderator: 'Модератор',
}
const roleLabel = (role) => roleLabels[role] || role

const getRoleTag = (role) => {
  if (role === 'moderator') return 'danger'
  if (role === 'organizer') return 'warning'
  return 'success'
}

const loadUsers = async () => {
  loading.value = true
  try {
    users.value = await getModeratorUsers()
  } catch (error) {
    ElMessage.error(error.response?.data?.error || 'Не удалось загрузить пользователей')
  } finally {
    loading.value = false
  }
}

const openRoleDialog = (user) => {
  selectedUser.value = user
  newRole.value = user.role
  roleDialogVisible.value = true
}

const saveRole = async () => {
  if (!selectedUser.value) return
  saving.value = true
  try {
    await updateUserRole(selectedUser.value.id, newRole.value)
    selectedUser.value.role = newRole.value
    ElMessage.success('Роль пользователя обновлена')
    roleDialogVisible.value = false
  } catch (error) {
    ElMessage.error(error.response?.data?.error || 'Не удалось изменить роль')
  } finally {
    saving.value = false
  }
}

const toggleBlock = async (user) => {
  const block = !user.is_blocked
  try {
    await ElMessageBox.confirm(
      block
        ? `Ограничить доступ пользователю ${user.email}?`
        : `Восстановить доступ пользователю ${user.email}?`,
      'Подтверждение',
      { confirmButtonText: 'Да', cancelButtonText: 'Отмена', type: 'warning' }
    )
  } catch {
    return 
  }
  try {
    await setUserBlocked(user.id, block)
    user.is_blocked = block
    ElMessage.success(block ? 'Доступ ограничен' : 'Доступ восстановлен')
  } catch (error) {
    ElMessage.error(error.response?.data?.error || 'Не удалось изменить статус доступа')
  }
}

onMounted(loadUsers)
</script>

<style scoped lang="scss">
.moderator-panel {
  padding: 24px 40px;
}
.moderator-panel h2 {
  margin-bottom: 20px;
}
.dialog-user {
  margin: 0 0 16px;
  color: #6b7490;
  font-size: 14px;
}
</style>
