<template>
    <div class="moderator-page">
        <Header />
        <div class="moderator-panel">
            <h1 class="panel-title">{{ $t('moderator.queue.title') }}</h1>

            <el-table
                :data="users"
                v-loading="loading"
                style="width: 100%"
                border
                stripe
                :empty-text="$t('common.noResults')"
            >
                <el-table-column prop="email" :label="$t('moderator.queue.email')" min-width="200" />
                <el-table-column :label="$t('moderator.queue.name')" min-width="160">
                    <template #default="scope">
                        {{ scope.row.last_name }} {{ scope.row.first_name }}
                    </template>
                </el-table-column>
                <el-table-column prop="role" :label="$t('moderator.queue.role')" width="150">
                    <template #default="scope">
                        <el-tag :type="getRoleTag(scope.row.role)">
                            {{ roleLabel(scope.row.role) }}
                        </el-tag>
                    </template>
                </el-table-column>
                <el-table-column :label="$t('moderator.queue.status')" width="140">
                    <template #default="scope">
                        <el-tag :type="scope.row.is_blocked ? 'danger' : 'success'">
                            {{ scope.row.is_blocked ? $t('moderator.queue.blocked') : $t('moderator.queue.active') }}
                        </el-tag>
                    </template>
                </el-table-column>
                <el-table-column :label="$t('moderator.queue.actions')" width="280">
                    <template #default="scope">
                        <el-button size="small" @click="openRoleDialog(scope.row)">
                            {{ $t('moderator.queue.changeRole') }}
                        </el-button>
                        <el-button
                            size="small"
                            :type="scope.row.is_blocked ? 'success' : 'danger'"
                            @click="toggleBlock(scope.row)"
                            :loading="blockingId === scope.row.id"
                        >
                            {{ scope.row.is_blocked ? $t('moderator.queue.unblock') : $t('moderator.queue.block') }}
                        </el-button>
                    </template>
                </el-table-column>
            </el-table>

            <el-dialog
                v-model="roleDialogVisible"
                :title="$t('moderator.queue.dialog.title')"
                width="420px"
            >
                <p v-if="selectedUser" class="dialog-user">{{ selectedUser.email }}</p>
                <el-select v-model="newRole" :placeholder="$t('moderator.queue.role')" style="width: 100%">
                    <el-option :label="$t('moderator.queue.roles.participant')" value="participant" />
                    <el-option :label="$t('moderator.queue.roles.organizer')" value="organizer" />
                    <el-option :label="$t('moderator.queue.roles.moderator')" value="moderator" />
                </el-select>
                <template #footer>
                    <el-button @click="roleDialogVisible = false">
                        {{ $t('moderator.queue.dialog.cancelBtn') }}
                    </el-button>
                    <el-button type="primary" :loading="saving" @click="saveRole">
                        {{ $t('moderator.queue.dialog.saveBtn') }}
                    </el-button>
                </template>
            </el-dialog>
        </div>
    </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { useI18n } from 'vue-i18n'
import { ElMessage, ElMessageBox } from 'element-plus'
import Header from '@/views/commons/Header.vue'
import { getModeratorUsers, updateUserRole, setUserBlocked } from '@/services/api'

const { t } = useI18n()

const users = ref([])
const loading = ref(false)
const saving = ref(false)
const blockingId = ref(null)

const roleDialogVisible = ref(false)
const selectedUser = ref(null)
const newRole = ref('')

const roleLabel = (role) => {
    const labels = {
        participant: t('moderator.queue.roles.participant'),
        organizer: t('moderator.queue.roles.organizer'),
        moderator: t('moderator.queue.roles.moderator'),
    }
    return labels[role] || role
}

const getRoleTag = (role) => {
    if (role === 'moderator') return 'danger'
    if (role === 'organizer') return 'warning'
    return 'success'
}

const fetchUsers = async () => {
    loading.value = true
    try {
        users.value = await getModeratorUsers()
    } catch (e) {
        ElMessage.error(t('common.error'))
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
    if (!selectedUser.value || !newRole.value) return
    saving.value = true
    try {
        await updateUserRole(selectedUser.value.id, newRole.value)
        selectedUser.value.role = newRole.value
        ElMessage.success(t('common.success'))
        roleDialogVisible.value = false
    } catch (e) {
        ElMessage.error(t('common.error'))
    } finally {
        saving.value = false
    }
}

const toggleBlock = async (user) => {
    try {
        await ElMessageBox.confirm(
            user.is_blocked
                ? t('moderator.queue.unblock') + '?'
                : t('moderator.queue.block') + '?',
            { type: 'warning', confirmButtonText: t('common.yes'), cancelButtonText: t('common.cancel') }
        )
        blockingId.value = user.id
        await setUserBlocked(user.id, !user.is_blocked)
        user.is_blocked = !user.is_blocked
        ElMessage.success(t('common.success'))
    } catch {
        // cancelled
    } finally {
        blockingId.value = null
    }
}

onMounted(fetchUsers)
</script>

<style scoped lang="scss">
.moderator-page {
    min-height: 100vh;
    background-color: variables.$dark-white;
}

.moderator-panel {
    max-width: 1200px;
    margin: 0 auto;
    padding: 32px 40px;
}

.panel-title {
    font-size: 26px;
    font-weight: 700;
    color: variables.$black;
    margin: 0 0 24px;
}

.dialog-user {
    color: variables.$gray;
    font-size: 14px;
    margin-bottom: 16px;
    word-break: break-all;
}
</style>